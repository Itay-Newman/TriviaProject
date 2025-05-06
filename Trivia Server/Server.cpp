#include "Server.h"
#include <WinSock2.h>

Server::Server() : m_isRunning(false), m_communicator(nullptr), m_handlerFactory(nullptr), m_database(nullptr)
{
    // Initialize database
    m_database = new SqliteDataBase("triviaDB.sqlite");

    // Initialize request handler factory
    m_handlerFactory = new RequestHandlerFactory(*m_database);

    // Initialize communicator with the request handler factory
    m_communicator = new Communicator(*m_handlerFactory);
}

Server::~Server()
{
    close();

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
}

void Server::run()
{
    m_isRunning = true;

    // Start console thread for handling user input
    m_consoleThread = std::thread(&Server::handleUserInput, this);

    // Start handling client requests in the communicator
    m_communicator->startHandleRequests();

    // Wait for console thread to finish
    if (m_consoleThread.joinable())
    {
        m_consoleThread.join();
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

    // Force console thread to join if it's still running
    if (m_consoleThread.joinable())
    {
        m_consoleThread.join();
    }
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

            // Force the communicator to close by making a local connection
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