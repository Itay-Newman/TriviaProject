#include "RequestHandlerFactory.h"

LoginRequestHandler* RequestHandlerFactory::createLoginRequstHandler()
{
	auto* handler = new LoginRequestHandler(m_Database, m_LoginManager);
	return handler;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_LoginManager;
}