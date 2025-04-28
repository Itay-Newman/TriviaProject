#pragma once

#include "LoginRequestHandler.h"
#include "IRequestHandler.h"
#include "IDatabase.h"
#include "LoginManager.h"

class RequestHandlerFactory
{
public:
	RequestHandlerFactory();
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler createLoginRequestHandler();

	LoginManager& getLoginManager();

private:
	IDatabase* m_Database;
	LoginManager m_LoginManager;
};
