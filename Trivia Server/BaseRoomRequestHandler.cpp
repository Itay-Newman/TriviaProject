#include "BaseRoomRequestHandler.h"

BaseRoomRequestHandler::BaseRoomRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, RequestHandlerFactory* HandlerFactory)
	: m_database(database), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_HandlerFactory(*HandlerFactory)
{
}

bool BaseRoomRequestHandler::doesUserExist(const std::string& username) const
{
	return m_database.doesUserExist(username);
}

bool BaseRoomRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_database.doesPasswordMatch(username, password);
}

bool BaseRoomRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_database.addUser(username, password, email);
}

RequestResult BaseRoomRequestHandler::createErrorResponse(const std::string& message)
{
	ErrorResponse errorResponse;
	errorResponse.status = ResponseCode::ERROR_RESPONSE;
	errorResponse.message = message;

	RequestResult result;
	result.id = ResponseCode::ERROR_RESPONSE;
	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = this;

	return result;
}