#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

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
    RoomManager& getRoomManager();
    StatisticsManager& getStatisticsManager();

private:
    IDatabase& m_Database;
    LoginManager* m_LoginManager;
    RoomManager* m_RoomManager;
    StatisticsManager* m_StatisticsManager;
};