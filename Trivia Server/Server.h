#pragma once

#include <iostream>
#include <thread>
#include <string>
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"

class Server
{
public:
    Server();
    ~Server();

    void run();
    void close();

private:
    Communicator* m_communicator;
    IDatabase* m_database;
    RequestHandlerFactory* m_handlerFactory;
    bool m_isRunning;
    std::thread m_consoleThread;

    void handleUserInput();
};