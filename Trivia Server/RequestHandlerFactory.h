#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

class LoginRequestHandler;
class Communicator;

class RequestHandlerFactory
{
public:
	explicit RequestHandlerFactory(IDatabase& database);
	~RequestHandlerFactory();

	void setCommunicator(Communicator* communicator);

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const std::string& username);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const std::string& username);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const std::string& username);

	LoginManager& getLoginManager();
	IDatabase& getDataBase();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	Communicator* getCommunicator();

private:
	IDatabase& m_Database;
	LoginManager* m_LoginManager;
	RoomManager* m_RoomManager;
	StatisticsManager* m_statisticsManager;
	Communicator* m_communicator;
};