#pragma once

#include "BaseRoomRequestHandler.h"
#include "RequestHandlerFactory.h"

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