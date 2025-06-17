#include "Communicator.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include <windows.h>

Communicator::Communicator(RequestHandlerFactory& handlerFactory)
	: m_serverSocket(INVALID_SOCKET), m_isRunning(false), m_handlerFactory(handlerFactory)
{
}

Communicator::~Communicator()
{
	close();
}

void Communicator::startHandleRequests()
{
	if (!initializeWinsock())
	{
		std::cout << "Failed to initialize Winsock" << std::endl;
		return;
	}

	if (!bindAndListen())
	{
		std::cout << "Failed to bind and listen on socket" << std::endl;
		return;
	}

	m_isRunning = true;

	// Loop for accepting clients
	while (m_isRunning)
	{
		// Accept a client and handle it
		SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			if (m_isRunning) // Showing the error only if still running
			{
				std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
			}
			continue;
		}

		std::cout << "Client connected. Socket: " << clientSocket << std::endl;

		LoginRequestHandler* handler = m_handlerFactory.createLoginRequestHandler();

		// Creating a thread to handle this client
		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			m_clients[clientSocket] = handler;
		}
		m_clientThreads.push_back(std::thread(&Communicator::handleNewClient, this, clientSocket));
	}

	// Waiting for all client threads to finish
	for (auto& t : m_clientThreads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

void Communicator::close()
{
	m_isRunning = false;

	// Close all client sockets
	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);
		for (auto& client : m_clients)
		{
			closesocket(client.first);
			delete client.second;
		}
		m_clients.clear();
	}

	{
		std::lock_guard<std::mutex> lock(m_userSocketsMutex);
		m_userSockets.clear();
	}

	// Close server socket
	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
		m_serverSocket = INVALID_SOCKET;
	}

	WSACleanup();
}

void Communicator::sendMessageToUser(const std::string& username, int messageCode, const std::vector<unsigned char>& buffer)
{
	std::lock_guard<std::mutex> lock(m_userSocketsMutex);
	auto it = m_userSockets.find(username);
	if (it != m_userSockets.end())
	{
		try
		{
			sendResponse(it->second, messageCode, buffer);
			std::cout << "Sent message to user " << username << " (socket " << it->second << ")" << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to send message to user " << username << ": " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "User " << username << " not found in active connections" << std::endl;
	}
}

void Communicator::sendMessageToUsers(const std::vector<std::string>& usernames, int messageCode, const std::vector<unsigned char>& buffer)
{
	for (const std::string& username : usernames)
	{
		sendMessageToUser(username, messageCode, buffer);
	}
}

void Communicator::registerUserSocket(const std::string& username, SOCKET socket)
{
	std::lock_guard<std::mutex> lock(m_userSocketsMutex);
	m_userSockets[username] = socket;
	std::cout << "Registered user " << username << " with socket " << socket << std::endl;
}

void Communicator::unregisterUserSocket(const std::string& username)
{
	std::lock_guard<std::mutex> lock(m_userSocketsMutex);
	auto it = m_userSockets.find(username);
	if (it != m_userSockets.end())
	{
		std::cout << "Unregistered user " << username << " from socket " << it->second << std::endl;
		m_userSockets.erase(it);
	}
}

bool Communicator::initializeWinsock()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		std::cerr << "WSAStartup failed with error: " << result << std::endl;
		return false;
	}
	return true;
}

bool Communicator::bindAndListen()
{
	// Creating socket
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	// Address info
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET; // IPv4
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(8888); // Using port 8888

	// Binding the socket
	if (bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(m_serverSocket);
		WSACleanup();
		return false;
	}

	// Listening for incoming connections
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listening failed with error: " << WSAGetLastError() << std::endl;
		closesocket(m_serverSocket);
		WSACleanup();
		return false;
	}

	std::cout << "Server started. Listening on port 8888..." << std::endl;
	return true;
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		// Get the handler for this client
		IRequestHandler* handler;
		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			handler = m_clients[clientSocket];
		}

		bool clientConnected = true;
		while (m_isRunning && clientConnected)
		{
			try
			{
				// Get request from client
				RequestInfo requestInfo = getRequestFromClient(clientSocket);

				std::cout << "Received request from client " << clientSocket
					<< ", message code: " << requestInfo.id
					<< ", buffer size: " << requestInfo.buffer.size() << std::endl;

				std::string messageContent(requestInfo.buffer.begin(), requestInfo.buffer.end());
				std::cout << "Message content: " << messageContent << std::endl;

				// Check if request is relevant to the current handler
				if (!handler->isRequestRelevant(requestInfo))
				{
					ErrorResponse errorResponse;
					errorResponse.message = "Request not relevant to current handler";
					std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);

					sendResponse(clientSocket, 0, buffer);
					std::cout << "Sent error response to client " << clientSocket << ": Request not relevant" << std::endl;
					continue;
				}

				// Handle the request
				RequestResult responseInfo = handler->handleRequest(requestInfo);

				// Send response to client
				sendResponse(clientSocket, static_cast<int>(responseInfo.id), responseInfo.response);
				std::cout << "Sent response to client " << clientSocket
					<< ", message code: " << static_cast<int>(responseInfo.id)
					<< ", buffer size: " << responseInfo.response.size() << std::endl;

				if (responseInfo.newHandler != nullptr && responseInfo.newHandler != handler)
				{
					delete handler; // Clean up the old handler
					handler = responseInfo.newHandler;
					{
						std::lock_guard<std::mutex> lock(m_clientsMutex);
						m_clients[clientSocket] = handler;
					}
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error handling client request: " << e.what() << std::endl;
				clientConnected = false;
			}
		}

		// Client disconnected or server is shutting down
		std::cout << "Client disconnected. Socket: " << clientSocket << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in client handler: " << e.what() << std::endl;
	}

	closesocket(clientSocket);
}

std::vector<unsigned char> Communicator::getBufferFromSocket(SOCKET sc, int bytesToRead)
{
	std::vector<unsigned char> buffer(bytesToRead);
	int bytesReceived = 0;
	int result = 0;

	// Keep reading until we have all the data we need
	while (bytesReceived < bytesToRead)
	{
		result = recv(sc, reinterpret_cast<char*>(&buffer[bytesReceived]), bytesToRead - bytesReceived, 0);

		if (result == SOCKET_ERROR)
		{
			throw std::exception("Error receiving data from socket");
		}

		if (result == 0)
		{
			throw std::exception("Connection closed by client");
		}

		bytesReceived += result;
	}

	return buffer;
}

void Communicator::sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer)
{
	int bytesSent = 0;
	int bytesToSend = static_cast<int>(buffer.size());
	int result = 0;

	// Keep sending until all data is sent
	while (bytesSent < bytesToSend)
	{
		result = send(sc, reinterpret_cast<const char*>(&buffer[bytesSent]), bytesToSend - bytesSent, 0);

		if (result == SOCKET_ERROR)
		{
			throw std::exception("Error sending data to socket");
		}

		bytesSent += result;
	}
}

void Communicator::sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer)
{
	std::vector<unsigned char> fullResponse;

	// Add message code (1 byte)
	fullResponse.push_back(static_cast<unsigned char>(messageCode));

	// Add message size (4 bytes)
	uint32_t messageSize = static_cast<uint32_t>(buffer.size());
	for (int i = 3; i >= 0; i--)
	{
		fullResponse.push_back((messageSize >> (i * 8)) & 0xFF);
	}

	// Add the actual message
	fullResponse.insert(fullResponse.end(), buffer.begin(), buffer.end());

	if (!buffer.empty())
	{
		std::string sentMessageContent(buffer.begin(), buffer.end());
		std::cout << "Message content (sent): " << sentMessageContent << std::endl;
	}

	// Send the full response
	sendBuffer(sc, fullResponse);
}

RequestInfo Communicator::getRequestFromClient(SOCKET clientSocket)
{
	RequestInfo requestInfo;
	requestInfo.receivalTime = std::chrono::system_clock::now();

	// Read message code (1 byte)
	std::vector<unsigned char> codeBuffer = getBufferFromSocket(clientSocket, 1);
	requestInfo.id = static_cast<int>(codeBuffer[0]);

	// Read message size (4 bytes)
	std::vector<unsigned char> sizeBuffer = getBufferFromSocket(clientSocket, 4);
	uint32_t messageSize = 0;
	for (int i = 0; i < 4; i++)
	{
		messageSize = (messageSize << 8) | sizeBuffer[i];
	}

	// Read the actual message
	if (messageSize > 0)
	{
		requestInfo.buffer = getBufferFromSocket(clientSocket, messageSize);
	}

	return requestInfo;
}