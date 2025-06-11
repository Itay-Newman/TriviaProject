#pragma once

#include "IRequestHandler.h"
#include "loginManager.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "BaseRoomRequestHandler.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username, RequestHandlerFactory& handlerFactory);
	~MenuRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	// Helper methods for handling specific request types
	RequestResult handleCreateRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleGetRoomsRequest(const RequestInfo& requestInfo);
	RequestResult handleGetPlayersInRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleJoinRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleGetStatisticsRequest(const RequestInfo& requestInfo);
	RequestResult handleLogoutRequest(const RequestInfo& requestInfo);

	IDatabase& m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

};