#pragma once

#include "IDatabase.h"
#include "LoginManager.h"

class LoginRequestHandler;

class RequestHandlerFactory
{
public:
	explicit RequestHandlerFactory(IDatabase* database);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();

	LoginManager& getLoginManager();
	IDatabase& getDataBase();

private:
	IDatabase* m_Database;
	LoginManager* m_LoginManager;
};
