// All things that should have been in the Communicator class are happening here in the server class
// And thats why there is no Communicator class (it replaces it)

#include "Server.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

Server::Server() : m_serverSocket(INVALID_SOCKET), m_isRunning(false)
{
}

Server::~Server()
{
    close();
}

void Server::start()
{
    if (!initializeWinsock())
    {
        return;
    }

    if (!bindAndListen())
    {
        return;
    }

    m_isRunning = true;

    // Starting thread for console input
    m_consoleThread = std::thread(&Server::handleUserInput, this);

    // Loop for accepting clients
    while (m_isRunning)
    {
        acceptClient();
    }

    // Waiting for console thread to finish
    if (m_consoleThread.joinable())
    {
        m_consoleThread.join();
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

void Server::close()
{
    m_isRunning = false;

    // Close all client sockets
    for (auto& client : m_clients)
    {
        closesocket(client.first);
        delete client.second;
    }
    m_clients.clear();

    // Close server socket
    if (m_serverSocket != INVALID_SOCKET)
    {
        closesocket(m_serverSocket);
        m_serverSocket = INVALID_SOCKET;
    }

    WSACleanup();
}

bool Server::initializeWinsock()
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

bool Server::bindAndListen()
{
    // Creating socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    // Setup address info
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
    std::cout << "Type 'EXIT' to close the server." << std::endl;
    return true;
}

bool Server::acceptClient()
{
    // Accepting client connection
    SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        if (m_isRunning) // Showing the error only if the server is still running
        {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        }
        return false;
    }

    std::cout << "Client connected. Socket: " << clientSocket << std::endl;

    // Creating a handler for this client
    IRequestHandler* handler = new LoginRequestHandler();

    m_clients[clientSocket] = handler;

    // Creating a thread to handle this client
    m_clientThreads.push_back(std::thread(&Server::handleNewClient, this, clientSocket));

    return true;
}

void Server::handleNewClient(SOCKET clientSocket)
{
    try
    {
        // Get the handler for this client
        IRequestHandler* handler = m_clients[clientSocket];

        bool clientConnected = true;
        while (m_isRunning && clientConnected)
        {
            try
            {
                // Get request from client
                RequestInfo requestInfo = getRequestFromClient(clientSocket);

                // Log the received request
                std::cout << "Received request from client " << clientSocket
                    << ", message code: " << requestInfo.id
                    << ", buffer size: " << requestInfo.buffer.size() << std::endl;
                // Print the actual message content
                if (!requestInfo.buffer.empty()) 
                {
                    std::string messageContent(requestInfo.buffer.begin(), requestInfo.buffer.end());
                    std::cout << "Message content: " << messageContent << std::endl;
                }

                // Check if request is relevant to the current handler
                if (!handler->isRequestRelevant(requestInfo))
                {
                    // Create error response for unsupported request types
                    ErrorResponse errorResponse;
                    errorResponse.message = "Request not relevant to current handler";
                    std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);

                    // Send error response
                    sendResponse(clientSocket, 0, buffer);
                    std::cout << "Sent error response to client " << clientSocket << ": Request not relevant" << std::endl;
                    continue;
                }

                // Handle the request
                RequestInfo responseInfo = handler->handleRequest(requestInfo);

                // Send response to client
                sendResponse(clientSocket, responseInfo.id, responseInfo.buffer);
                std::cout << "Sent response to client " << clientSocket
                    << ", message code: " << responseInfo.id
                    << ", buffer size: " << responseInfo.buffer.size() << std::endl;

                // If we need to update the handler, we could do it here
                // For now, we'll keep using the same handler
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

void Server::handleUserInput()
{
    std::string command;

    while (m_isRunning)
    {
        std::getline(std::cin, command);

        if (command == "EXIT")
        {
            std::cout << "Shutting down server..." << std::endl;
            m_isRunning = false;

            // Forcing the accept() to return by connecting to ourselves and making the server shut down
            SOCKET tempSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (tempSocket != INVALID_SOCKET)
            {
                sockaddr_in addr;
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = inet_addr("127.0.0.1");
                addr.sin_port = htons(8888);
                connect(tempSocket, (sockaddr*)&addr, sizeof(addr));
                closesocket(tempSocket);
            }

            break;
        }
    }
}

std::vector<unsigned char> Server::getBufferFromSocket(SOCKET sc, int bytesToRead)
{
    std::vector<unsigned char> buffer(bytesToRead);
    int bytesReceived = 0;
    int result;

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

void Server::sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer)
{
    int bytesSent = 0;
    int bytesToSend = buffer.size();
    int result;

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

void Server::sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer)
{
    // Message structure:
    // 1 byte - message code
    // 4 bytes - message size
    // [message size] bytes - the actual message

    std::vector<unsigned char> fullResponse;

    // Add message code (1 byte)
    fullResponse.push_back(static_cast<unsigned char>(messageCode));

    // Add message size (4 bytes)
    uint32_t messageSize = buffer.size();
    for (int i = 3; i >= 0; i--)
    {
        fullResponse.push_back((messageSize >> (i * 8)) & 0xFF);
    }

    // Add the actual message
    fullResponse.insert(fullResponse.end(), buffer.begin(), buffer.end());

    // Send the full response
    sendBuffer(sc, fullResponse);
}

RequestInfo Server::getRequestFromClient(SOCKET clientSocket)
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