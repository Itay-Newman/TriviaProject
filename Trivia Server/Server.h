#pragma once

#include <iostream>
#include <thread>
#include <string>
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"
#include <mutex>

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
	std::mutex m_mutex;
	std::thread m_consoleThread;

	void handleUserInput();

	static constexpr const char* DEFAULT_IP = "127.0.0.1";
	static constexpr unsigned short DEFAULT_PORT = 8888;

	bool getIsRunning();
	void setIsRunning(bool value);
};