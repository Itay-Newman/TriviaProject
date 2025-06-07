#pragma once

#include "IRequestHandler.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username);
	~RoomAdminRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	RequestResult handleCloseRoomRequest(const RequestInfo& requestInfo);
	RequestResult handleStartGameRequest(const RequestInfo& requestInfo);
	RequestResult handleGetRoomStateRequest(const RequestInfo& requestInfo);

	IDatabase& m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	std::string m_username;
};