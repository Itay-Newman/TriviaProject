#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase& database)
	: m_Database(database)
{
	m_LoginManager = new LoginManager(database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this, this->m_Database);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler(this->m_Database, this->m_LoginManager);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return *this->m_LoginManager;
}

IDatabase& RequestHandlerFactory::getDataBase()
{
	return this->m_Database;
}
