#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory()
{
	// Default constructor  
	// Initialize with a null database pointer  
	this->m_Database = nullptr;
	this->m_LoginManager = LoginManager(nullptr); // Use direct initialization instead of new  
}

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
