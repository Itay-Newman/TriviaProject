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

class Communicator
{
public:
    Communicator(RequestHandlerFactory& handlerFactory);
    ~Communicator();

    void startHandleRequests();
    void close();

private:
    SOCKET m_serverSocket;
    std::map<SOCKET, IRequestHandler*> m_clients;
    std::vector<std::thread> m_clientThreads;
    RequestHandlerFactory& m_handlerFactory;
    bool m_isRunning;

    bool bindAndListen();
    void handleNewClient(SOCKET clientSocket);
    std::vector<unsigned char> getBufferFromSocket(SOCKET sc, int bytesToRead);
    void sendBuffer(SOCKET sc, const std::vector<unsigned char>& buffer);
    void sendResponse(SOCKET sc, int messageCode, const std::vector<unsigned char>& buffer);
    RequestInfo getRequestFromClient(SOCKET clientSocket);
    bool initializeWinsock();
};