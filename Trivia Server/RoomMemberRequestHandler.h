#pragma once

#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

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