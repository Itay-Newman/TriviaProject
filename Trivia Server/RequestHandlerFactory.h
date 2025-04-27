#pragma once

#include "LoginRequestHandler.h"
#include "IRequestHandler.h"

class RequestHandlerFactory
{
public:
	LoginRequestHandler* createLoginRequstHandler();
	LoginManager& getLoginManager();
private:
	LoginManager m_LoginManager;
	IRequestHandler* m_Database;
};