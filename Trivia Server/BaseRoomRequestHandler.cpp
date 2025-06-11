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

RequestResult BaseRoomRequestHandler::handleGetRoomStateRequest(const RequestInfo& requestInfo)
{
	try
	{
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(this->m_username);

		JsonRequestPacketDeserializer deserializer;
		GetRoomStateRequest request = deserializer.deserializeGetRoomStateRequest(requestInfo.buffer);

		RoomState roomState = m_roomManager->getRoomState(roomIdOpt.value());
		std::vector<std::string> players = m_roomManager->getUsersInRoom(roomIdOpt.value());

		auto maybeRoom = m_roomManager->getRoom(roomIdOpt.value());

		GetRoomStateResponse response
		{
			.status = (unsigned int)Status::SUCCESS,
			.hasGameBegun = (roomState == RoomState::GAME_IN_PROGRESS),
			.players = players
		};

		if (maybeRoom)
		{
			Room& room = maybeRoom->get();
			response.questionCount = room.getNumOfQuestionsInGame();
			response.answerTimeout = room.getTimePerQuestion();
		}
		else
		{
			response.questionCount = 0;
			response.answerTimeout = 0;
		}

		RequestResult result;
		result.id = ResponseCode::GET_ROOM_STATE_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
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