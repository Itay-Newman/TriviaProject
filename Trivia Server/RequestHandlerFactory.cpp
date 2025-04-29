#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database)
	: m_Database(database)
{
	m_LoginManager = new LoginManager(database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	delete m_LoginManager;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return *m_LoginManager;
}

IDatabase& RequestHandlerFactory::getDataBase()
{
	return *this->m_Database;
}