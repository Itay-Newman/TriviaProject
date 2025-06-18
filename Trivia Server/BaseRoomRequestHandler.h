#pragma once

#include "IRequestHandler.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "communicator.h"
#include "Server.h"

class RequestHandlerFactory;

//! In order to avoid circular dependencies, all the handlers that are about the user but before the game, will be here
// A class to solve code duplication between RoomAdminRequestHandler and RoomMemberRequestHandler

class BaseRoomRequestHandler : public IRequestHandler
{
public:
	BaseRoomRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, RequestHandlerFactory* HandlerFactory);
	virtual ~BaseRoomRequestHandler() = default;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

protected:
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);
	RequestResult createErrorResponse(const std::string& message);


	std::string m_username;
	IDatabase& m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	RequestHandlerFactory& m_HandlerFactory;
};



class RoomAdminRequestHandler : public BaseRoomRequestHandler
{
public:
	RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username, RequestHandlerFactory* handlerFactory);
	~RoomAdminRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo);
	virtual RequestResult handleRequest(const RequestInfo& requestInfo);

private:
	RequestResult handleCloseRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleStartGameRequest(const RequestInfo& requestInfo);
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);
};



class RoomMemberRequestHandler : public BaseRoomRequestHandler
{
public:
	RoomMemberRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username, RequestHandlerFactory* handlerFactory);
	~RoomMemberRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo);
	virtual RequestResult handleRequest(const RequestInfo& requestInfo);

private:
	RequestResult handleLeaveRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);
};