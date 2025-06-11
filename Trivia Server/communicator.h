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
#include <mutex>

class RequestHandlerFactory;

#pragma comment(lib, "ws2_32.lib")

class Communicator
{
public:
	Communicator(RequestHandlerFactory& handlerFactory);
	~Communicator();

	void startHandleRequests();
	void close();

	// methods for broadcasting messages
	void sendMessageToUser(const std::string& username, int messageCode, const std::vector<unsigned char>& buffer);
	void sendMessageToUsers(const std::vector<std::string>& usernames, int messageCode, const std::vector<unsigned char>& buffer);

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::map<std::string, SOCKET> m_userSockets;
	std::vector<std::thread> m_clientThreads;
	RequestHandlerFactory& m_handlerFactory;
	bool m_isRunning;

	std::mutex m_clientsMutex;
	std::mutex m_clientThreadsMutex;
	std::mutex m_userSocketsMutex;

	bool bindAndListen();
	void handleNewClient(SOCKET clientSocket);
	std::vector<unsigned char> getBufferFromSocket(SOCKET sc, int bytesToRead);
	void sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer);
	void sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer);
	RequestInfo getRequestFromClient(SOCKET clientSocket);
	bool initializeWinsock();

	void registerUserSocket(const std::string& username, SOCKET socket);
	void unregisterUserSocket(const std::string& username);
};