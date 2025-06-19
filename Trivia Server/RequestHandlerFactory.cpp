#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "BaseRoomRequestHandler.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"
#include "GameManager.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase& database)
	: m_Database(database)
{
	this->m_LoginManager = new LoginManager(this->m_Database);
	this->m_RoomManager = new RoomManager();
	this->m_statisticsManager = new StatisticsManager(this->m_Database);
	this->m_gameManager = new GameManager(this->m_Database);
	this->m_communicator = new Communicator(*this);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	delete this->m_LoginManager;
	delete this->m_RoomManager;
	delete this->m_statisticsManager;
	delete this->m_gameManager;
}

void RequestHandlerFactory::setCommunicator(Communicator* communicator)
{
	m_communicator = communicator;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this, this->m_Database);
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string& username)
{
	return new MenuRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		username,
		*this
	);
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const std::string& username)
{
	return new RoomAdminRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		username,
		this
	);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const std::string& username)
{
	return new RoomMemberRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		username,
		this
	);
}

IRequestHandler* RequestHandlerFactory::createGameRequestHandler(const std::string& username)
{
	return new GameRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		this->m_gameManager,
		username,
		*this
	);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return *this->m_LoginManager;
}

IDatabase& RequestHandlerFactory::getDataBase()
{
	return this->m_Database;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return *this->m_RoomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return *this->m_statisticsManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return *this->m_gameManager;
}

Communicator* RequestHandlerFactory::getCommunicator()
{
	return this->m_communicator;
}