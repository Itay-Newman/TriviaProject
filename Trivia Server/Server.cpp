#include "Server.h"
#include <WinSock2.h>
#include <ws2tcpip.h>

Server::Server() : m_isRunning(false), m_communicator(nullptr), m_handlerFactory(nullptr), m_database(nullptr)
{
	WSADATA wsaData;
	int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != 0)
	{
		std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
		exit(EXIT_FAILURE);
	}

	m_database = new SqliteDataBase("triviaDB.sqlite");
	m_handlerFactory = new RequestHandlerFactory(*m_database);
	m_communicator = new Communicator(*m_handlerFactory);
}

Server::~Server()
{
	close();

	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_consoleThread.joinable())
		m_consoleThread.join();

	// Clean up resources
	if (m_communicator != nullptr)
	{
		delete m_communicator;
		m_communicator = nullptr;
	}

	if (m_handlerFactory != nullptr)
	{
		delete m_handlerFactory;
		m_handlerFactory = nullptr;
	}

	if (m_database != nullptr)
	{
		delete m_database;
		m_database = nullptr;
	}

	WSACleanup();
}

void Server::run()
{
	setIsRunning(true);

	std::lock_guard<std::mutex> lock(m_mutex);
	m_consoleThread = std::thread(&Server::handleUserInput, this);

	// Start handling client requests in the communicator
	m_communicator->startHandleRequests();

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_consoleThread.joinable())
		{
			m_consoleThread.join();
		}
	}
}

void Server::close()
{
	m_isRunning = false;

	// Close the communicator
	if (m_communicator != nullptr)
	{
		m_communicator->close();
	}

	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_consoleThread.joinable())
	{
		m_consoleThread.join();
	}
}

void Server::handleUserInput()
{
	std::string command;

	while (getIsRunning())
	{
		if (!std::getline(std::cin, command))
			break;

		if (command == "EXIT")
		{
			std::cout << "Shutting down server..." << std::endl;
			setIsRunning(false);

			SOCKET tempSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (tempSocket == INVALID_SOCKET)
			{
				std::cerr << "Failed to create temp socket, error: " << WSAGetLastError() << std::endl;
				continue;
			}

			sockaddr_in addr = {};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(DEFAULT_PORT);

			int res = inet_pton(AF_INET, DEFAULT_IP, &addr.sin_addr);
			if (res != 1)
			{
				std::cerr << "Invalid IP address: " << DEFAULT_IP << std::endl;
				closesocket(tempSocket);
				continue;
			}

			if (connect(tempSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				std::cerr << "Failed to connect temp socket, error: " << err << std::endl;
			}

			closesocket(tempSocket);

			break;
		}
	}
}

bool Server::getIsRunning()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_isRunning;
}

void Server::setIsRunning(bool value)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isRunning = value;
}
