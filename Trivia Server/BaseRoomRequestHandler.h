#pragma once

#include "IRequestHandler.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

// A class to solve code duplication between RoomAdminRequestHandler and RoomMemberRequestHandler
class BaseRoomRequestHandler : public IRequestHandler
{
public:
	BaseRoomRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager);
	virtual ~BaseRoomRequestHandler() = default;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

protected:
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);
	RequestResult createErrorResponse(const std::string& message);

	IDatabase& m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
};