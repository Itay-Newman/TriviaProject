#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username)
	: BaseRoomRequestHandler(database, roomManager, statisticsManager), m_username(username)
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
			return createErrorResponse("Request type not supported");
		}
	}

	return createErrorResponse("Invalid request format");
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
		return createErrorResponse(e.what());
	}
}