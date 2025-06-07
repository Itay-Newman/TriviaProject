#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username)
	: m_database(database), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_username(username)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.buffer.size() >= 1)
	{
		unsigned char code = requestInfo.buffer[0];
		return code == static_cast<unsigned char>(RequestCodes::CLOSE_ROOM_REQUEST) ||
			code == static_cast<unsigned char>(RequestCodes::START_GAME_REQUEST) ||
			code == static_cast<unsigned char>(RequestCodes::GET_ROOM_STATE_REQUEST);
	}
	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (requestInfo.buffer.size() >= 1)
	{
		unsigned char code = requestInfo.buffer[0];

		switch (static_cast<RequestCodes>(code))
		{
		case RequestCodes::CLOSE_ROOM_REQUEST:
			return handleCloseRoomRequest(requestInfo);
		case RequestCodes::START_GAME_REQUEST:
			return handleStartGameRequest(requestInfo);
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

bool RoomAdminRequestHandler::doesUserExist(const std::string& username) const
{
	return m_database.doesUserExist(username);
}

bool RoomAdminRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_database.doesPasswordMatch(username, password);
}

bool RoomAdminRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_database.addUser(username, password, email);
}

RequestResult RoomAdminRequestHandler::handleCloseRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		JsonRequestPacketDeserializer deserializer;
		CloseRoomRequest closeRoomRequest = deserializer.deserializeCloseRoomRequest(requestInfo.buffer);

		std::vector<std::string> usersInRoom = m_roomManager->getUsersInRoom(closeRoomRequest.roomId);

		bool success = m_roomManager->deleteRoom(closeRoomRequest.roomId);

		// Preparing the response
		CloseRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::CLOSE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		// Sending LeaveRoomResponse to all the room members (if succesful...)
		if (success)
		{
			// Need to send LeaveRoomResponse to all users in usersInRoom through the server/communicator
		}

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

RequestResult RoomAdminRequestHandler::handleStartGameRequest(const RequestInfo& requestInfo)
{
	try
	{
		StartGameResponse response;
		response.status = (unsigned int)Status::SUCCESS;

		RequestResult result;
		result.id = ResponseCode::START_GAME_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this; // Could be replaced to a GameRequestHandler in the future

		// Need to send StartGameResponse to all room members through the server/communicator

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

RequestResult RoomAdminRequestHandler::handleGetRoomStateRequest(const RequestInfo& requestInfo)
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

		// Serialize the response
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