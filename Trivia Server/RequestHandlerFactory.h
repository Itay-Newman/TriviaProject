#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "BaseRoomRequestHandler.h"
#include "IRequestHandler.h"

class LoginRequestHandler;
class Communicator;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class MenuRequestHandler;
class GameRequestHandler;
class GameManager;

class RequestHandlerFactory
{
public:
	explicit RequestHandlerFactory(IDatabase& database);
	~RequestHandlerFactory();

	void setCommunicator(Communicator* communicator);

	LoginRequestHandler* createLoginRequestHandler();
	IRequestHandler* createMenuRequestHandler(const std::string& username);
	IRequestHandler* createRoomAdminRequestHandler(const std::string& username);
	IRequestHandler* createRoomMemberRequestHandler(const std::string& username);
	IRequestHandler* createGameRequestHandler(const std::string& username);

	LoginManager& getLoginManager();
	IDatabase& getDataBase();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	GameManager& getGameManager();
	Communicator* getCommunicator();

private:
	IDatabase& m_Database;
	LoginManager* m_LoginManager;
	RoomManager* m_RoomManager;
	StatisticsManager* m_statisticsManager;
	GameManager* m_gameManager;
	Communicator* m_communicator;
};