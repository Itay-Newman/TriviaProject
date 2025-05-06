#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"

class LoginRequestHandler;

class RequestHandlerFactory
{
public:
	explicit RequestHandlerFactory(IDatabase& database);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();

	LoginManager& getLoginManager();
	IDatabase& getDataBase();

private:
	IDatabase& m_Database;
	LoginManager* m_LoginManager;
};
