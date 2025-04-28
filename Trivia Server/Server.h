#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include <string>
#include <Windows.h>
#include "RequestHandlerFactory.h"

#pragma comment(lib, "ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	void start();
	void close();

private:
	SOCKET m_serverSocket;
	bool m_isRunning;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::vector<std::thread> m_clientThreads;
	std::thread m_consoleThread;

	bool initializeWinsock();
	bool bindAndListen();
	bool acceptClient();
	void handleNewClient(SOCKET clientSocket);
	void handleUserInput();

	std::vector<unsigned char> getBufferFromSocket(SOCKET sc, int bytesToRead);
	void sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer);
	void sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer);
	RequestInfo getRequestFromClient(SOCKET clientSocket);

	RequestHandlerFactory* m_handlerFactory;
};