#pragma once

#include "BaseRoomRequestHandler.h"

class RoomAdminRequestHandler : public BaseRoomRequestHandler
{
public:
	RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager);
	~RoomAdminRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestResult handleCloseRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleStartGameRequest(const RequestInfo& requestInfo);
};