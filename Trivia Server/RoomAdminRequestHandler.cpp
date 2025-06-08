#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager)
	: BaseRoomRequestHandler(database, roomManager, statisticsManager)
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
			return createErrorResponse("Request type not supported");
		}
	}

	return createErrorResponse("Invalid request format");
}

RequestResult RoomAdminRequestHandler::handleCloseRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		JsonRequestPacketDeserializer deserializer;
		CloseRoomRequest closeRoomRequest = deserializer.deserializeCloseRoomRequest(requestInfo.buffer);

		// Get users in room before deleting the room
		std::vector<std::string> usersInRoom = m_roomManager->getUsersInRoom(closeRoomRequest.roomId);

		bool success = m_roomManager->deleteRoom(closeRoomRequest.roomId);

		CloseRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::CLOSE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		// Send LeaveRoomResponse to all room members
		if (success)
		{
			LeaveRoomResponse leaveResponse;
			leaveResponse.status = (unsigned int)Status::SUCCESS;
			std::vector<unsigned char> leaveResponseBuffer = JsonResponsePacketSerializer::serializeResponse(leaveResponse);

			// havnt done yet: Send leaveResponseBuffer to all users in usersInRoom through the server/communicator
			// This would typically be done through a callback or notification system
		}

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult RoomAdminRequestHandler::handleStartGameRequest(const RequestInfo& requestInfo)
{
	try
	{
		// StartGameRequest has no data, only request code (similar to CloseRoomRequest pattern)
		// We need to determine which room to start - this might need additional context
		// For now, creating a successful response

		StartGameResponse response;
		response.status = (unsigned int)Status::SUCCESS;

		RequestResult result;
		result.id = ResponseCode::START_GAME_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this; // Could be replaced with a GameRequestHandler in the future

		// havnt done yet: Send StartGameResponse to all room members
		// Need room ID to get users and notify them about game start
		// This requires additional context about which room the admin is starting

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}