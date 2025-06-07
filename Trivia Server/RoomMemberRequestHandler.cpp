#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username)
	: m_database(database), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_username(username)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.buffer.size() >= 1)
	{
		unsigned char code = requestInfo.buffer[0];
		return code == static_cast<unsigned char>(RequestCodes::LEAVE_ROOM_REQUEST) ||
			code == static_cast<unsigned char>(RequestCodes::GET_ROOM_STATE_REQUEST);
	}
	return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (requestInfo.buffer.size() >= 1)
	{
		unsigned char code = requestInfo.buffer[0];

		switch (static_cast<RequestCodes>(code))
		{
		case RequestCodes::LEAVE_ROOM_REQUEST:
			return handleLeaveRoomRequest(requestInfo);
		case RequestCodes::GET_ROOM_STATE_REQUEST:
			return handleGetRoomStateRequest(requestInfo);
		default:
			ErrorResponse errorResponse;
			errorResponse.status = ResponseCode::ERROR_RESPONSE;
			errorResponse.message = "Request type not supported";

			RequestResult result;
			result.id = ResponseCode::ERROR_RESPONSE;
			result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
			result.newHandler = this;
			return result;
		}
	}

	ErrorResponse errorResponse;
	errorResponse.status = ResponseCode::ERROR_RESPONSE;
	errorResponse.message = "Invalid request format";

	RequestResult result;
	result.id = ResponseCode::ERROR_RESPONSE;
	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = this;
	return result;
}

bool RoomMemberRequestHandler::doesUserExist(const std::string& username) const
{
	return m_database.doesUserExist(username);
}

bool RoomMemberRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_database.doesPasswordMatch(username, password);
}

bool RoomMemberRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_database.addUser(username, password, email);
}

RequestResult RoomMemberRequestHandler::handleLeaveRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		JsonRequestPacketDeserializer deserializer;
		LeaveRoomRequest leaveRoomRequest = deserializer.deserializeLeaveRoomRequest(requestInfo.buffer);

		bool success = m_roomManager->removeUserFromRoom(leaveRoomRequest.roomId, m_username);

		LeaveRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::LEAVE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		ErrorResponse errorResponse;
		errorResponse.status = ResponseCode::ERROR_RESPONSE;
		errorResponse.message = e.what();

		RequestResult result;
		result.id = ResponseCode::ERROR_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this;

		return result;
	}
}

RequestResult RoomMemberRequestHandler::handleGetRoomStateRequest(const RequestInfo& requestInfo)
{
	try
	{
		JsonRequestPacketDeserializer deserializer;
		GetRoomStateRequest request = deserializer.deserializeGetRoomStateRequest(requestInfo.buffer);

		RoomState roomState = m_roomManager->getRoomState(request.roomId);
		std::vector<std::string> players = m_roomManager->getUsersInRoom(request.roomId);

		auto maybeRoom = m_roomManager->getRoom(request.roomId);

		GetRoomStateResponse response;
		response.status = (unsigned int)Status::SUCCESS;
		response.hasGameBegun = (roomState == RoomState::GAME_IN_PROGRESS);
		response.players = players;

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
		ErrorResponse errorResponse;
		errorResponse.status = ResponseCode::ERROR_RESPONSE;
		errorResponse.message = e.what();

		RequestResult result;
		result.id = ResponseCode::ERROR_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		result.newHandler = this;

		return result;
	}
}