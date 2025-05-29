#include "Communicator.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

#define PORT 8888

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
	if (!initializeWinsock()) return;
	if (!bindAndListen()) return;

	m_isRunning = true;

	while (m_isRunning)
	{
		SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			if (m_isRunning)
				std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
			continue;
		}

		std::cout << "Client connected. Socket: " << clientSocket << std::endl;

		LoginRequestHandler* handler = m_handlerFactory.createLoginRequestHandler();

		// Store handler for this socket
		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			m_clients[clientSocket] = handler;
		}
		// Launch a thread to deal with the client
		{
			std::lock_guard<std::mutex> lock(m_clientThreadsMutex);
			m_clientThreads.emplace_back(&Communicator::handleNewClient, this, clientSocket);
		}
	}

	// Wait for all threads to finish before shutting down
	{
		std::lock_guard<std::mutex> lock(m_clientThreadsMutex);
		for (auto& t : m_clientThreads)
		{
			if (t.joinable()) t.join();
		}
		m_clientThreads.clear();
	}
}

void Communicator::close()
{
	m_isRunning = false;

	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);
		for (auto& client : m_clients)
		{
			closesocket(client.first);
			delete client.second;
		}
		m_clients.clear();
	}

	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
		m_serverSocket = INVALID_SOCKET;
	}

	WSACleanup();
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
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	if (bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(m_serverSocket);
		WSACleanup();
		return false;
	}

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
				RequestInfo requestInfo = getRequestFromClient(clientSocket);

				std::cout << "Received request from client " << clientSocket
					<< ", message code: " << requestInfo.id
					<< ", buffer size: " << requestInfo.buffer.size() << std::endl;

				if (!requestInfo.buffer.empty())
				{
					std::string messageContent(requestInfo.buffer.begin(), requestInfo.buffer.end());
					std::cout << "Message content: " << messageContent << std::endl;
				}

				if (!handler->isRequestRelevant(requestInfo))
				{
					ErrorResponse errorResponse;
					errorResponse.message = "Request not relevant to current handler";
					auto buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
					sendResponse(clientSocket, 0, buffer);
					std::cout << "Sent error response to client " << clientSocket << std::endl;
					continue;
				}

				RequestResult responseInfo = handler->handleRequest(requestInfo);
				sendResponse(clientSocket, static_cast<int>(responseInfo.id), responseInfo.response);
				std::cout << "Sent response to client " << clientSocket << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error handling client request: " << e.what() << std::endl;
				clientConnected = false;
			}
		}

		std::cout << "Client disconnected. Socket: " << clientSocket << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in client handler: " << e.what() << std::endl;
	}

	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);
		auto it = m_clients.find(clientSocket);
		if (it != m_clients.end())
		{
			delete it->second;
			m_clients.erase(it);
		}
	}

	closesocket(clientSocket);
}

std::vector<unsigned char> Communicator::getBufferFromSocket(SOCKET sc, int bytesToRead)
{
	std::vector<unsigned char> buffer(bytesToRead);
	int bytesReceived = 0;

	while (bytesReceived < bytesToRead)
	{
		int result = recv(sc, reinterpret_cast<char*>(&buffer[bytesReceived]), bytesToRead - bytesReceived, 0);
		if (result == SOCKET_ERROR) throw std::exception("Error receiving data from socket");
		if (result == 0) throw std::exception("Connection closed by client");
		bytesReceived += result;
	}

	return buffer;
}

void Communicator::sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer)
{
	int bytesSent = 0;
	int bytesToSend = buffer.size();

	while (bytesSent < bytesToSend)
	{
		int result = send(sc, reinterpret_cast<const char*>(&buffer[bytesSent]), bytesToSend - bytesSent, 0);
		if (result == SOCKET_ERROR) throw std::exception("Error sending data to socket");
		bytesSent += result;
	}
}

void Communicator::sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer)
{
	// Format: [1 byte code][4 byte size][payload]
	std::vector<unsigned char> fullResponse;
	fullResponse.push_back(static_cast<unsigned char>(messageCode));

	uint32_t messageSize = buffer.size();
	for (int i = 3; i >= 0; i--)
		fullResponse.push_back((messageSize >> (i * 8)) & 0xFF);

	fullResponse.insert(fullResponse.end(), buffer.begin(), buffer.end());

	sendBuffer(sc, fullResponse);
}

RequestInfo Communicator::getRequestFromClient(SOCKET clientSocket)
{
	RequestInfo requestInfo;
	requestInfo.receivalTime = std::chrono::system_clock::now();

	std::vector<unsigned char> codeBuffer = getBufferFromSocket(clientSocket, 1);
	requestInfo.id = static_cast<int>(codeBuffer[0]);

	std::vector<unsigned char> sizeBuffer = getBufferFromSocket(clientSocket, 4);
	uint32_t messageSize = 0;
	for (int i = 0; i < 4; i++)
		messageSize = (messageSize << 8) | sizeBuffer[i];

	if (messageSize > 1000000)
		throw std::exception("Message size too large, possible DOS attack");

	if (messageSize > 0)
		requestInfo.buffer = getBufferFromSocket(clientSocket, messageSize);

	return requestInfo;
}
