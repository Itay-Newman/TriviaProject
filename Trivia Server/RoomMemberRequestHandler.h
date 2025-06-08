#pragma once

#include "BaseRoomRequestHandler.h"

class RoomMemberRequestHandler : public BaseRoomRequestHandler
{
public:
	RoomMemberRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username);
	~RoomMemberRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestResult handleLeaveRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);

	std::string m_username;
};