#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase& database)
	: m_Database(database)
{
	this->m_LoginManager = new LoginManager(this->m_Database);
	this->m_RoomManager = new RoomManager();
	this->m_statisticsManager = new StatisticsManager(this->m_Database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	delete this->m_LoginManager;
	delete this->m_RoomManager;
	delete this->m_statisticsManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this, this->m_Database);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string& username)
{
	return new MenuRequestHandler(
		this->m_Database,
		this->m_LoginManager,
		this->m_RoomManager,
		this->m_statisticsManager,
		username
	);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const std::string& username)
{
	return new RoomAdminRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		username
	);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const std::string& username)
{
	return new RoomMemberRequestHandler(
		this->m_Database,
		this->m_RoomManager,
		this->m_statisticsManager,
		username
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