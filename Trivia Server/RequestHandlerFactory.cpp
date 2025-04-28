#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database)
	: m_Database(database), m_LoginManager(LoginManager(database))
{
}

LoginRequestHandler RequestHandlerFactory::createLoginRequestHandler()
{
	return LoginRequestHandler(m_LoginManager, *this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_LoginManager;
}
