#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include "IRequestHandler.h"

#pragma comment(lib, "ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	void start();
	void close();

private:
	void handleNewClient(SOCKET clientSocket);
	void handleReceivedMessages();
	void handleUserInput();

	bool initializeWinsock();
	bool bindAndListen();
	bool acceptClient();

	std::thread m_consoleThread;
	std::vector<std::thread> m_clientThreads;
	std::map<SOCKET, IRequestHandler*> m_clients;

	SOCKET m_serverSocket;
	bool m_isRunning;
};