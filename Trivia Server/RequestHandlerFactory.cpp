#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase& database)
    : m_Database(database)
{
    m_LoginManager = new LoginManager(database);
    m_RoomManager = new RoomManager();
    m_StatisticsManager = new StatisticsManager(database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
    delete m_LoginManager;
    delete m_RoomManager;
    delete m_StatisticsManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this, this->m_Database);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string& username)
{
    return new MenuRequestHandler(this->m_Database, this->m_LoginManager, this->m_RoomManager, this->m_StatisticsManager, username);
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
    return *this->m_StatisticsManager;
}