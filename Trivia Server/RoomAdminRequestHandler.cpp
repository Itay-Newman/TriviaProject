#include "RoomAdminRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Communicator.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager,
	const std::string& username, Communicator* communicator)
	: BaseRoomRequestHandler(database, roomManager, statisticsManager), m_username(username), m_communicator(communicator)
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
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_username);

		if (!roomIdOpt.has_value())
		{
			return createErrorResponse("User is not in any room");
		}

		unsigned int roomId = roomIdOpt.value();

		// Get all users in the room BEFORE deleting the room
		std::vector<std::string> usersInRoom = m_roomManager->getUsersInRoom(roomId);

		bool success = m_roomManager->deleteRoom(roomId);

		CloseRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::CLOSE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		// Send LeaveRoomResponse to all room members
		if (success && m_communicator != nullptr)
		{
			LeaveRoomResponse leaveResponse;
			leaveResponse.status = (unsigned int)Status::SUCCESS;
			std::vector<unsigned char> leaveResponseBuffer = JsonResponsePacketSerializer::serializeResponse(leaveResponse);

			// Send LeaveRoomResponse to all users in the room
			m_communicator->sendMessageToUsers(usersInRoom, static_cast<int>(ResponseCode::LEAVE_ROOM_RESPONSE), leaveResponseBuffer);

			std::cout << "Sent LeaveRoomResponse to all " << usersInRoom.size() << " users in room " << roomId << std::endl;
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
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_username);

		if (!roomIdOpt.has_value())
		{
			return createErrorResponse("User is not in any room");
		}

		unsigned int roomId = roomIdOpt.value();

		std::vector<std::string> usersInRoom = m_roomManager->getUsersInRoom(roomId);

		StartGameResponse response;
		response.status = (unsigned int)Status::SUCCESS;

		RequestResult result;
		result.id = ResponseCode::START_GAME_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this; // Will be replaced with a GameRequestHandler in the future

		// Send StartGameResponse to all room members
		if (m_communicator != nullptr)
		{
			StartGameResponse startGameResponse;
			startGameResponse.status = (unsigned int)Status::SUCCESS;
			std::vector<unsigned char> startGameResponseBuffer = JsonResponsePacketSerializer::serializeResponse(startGameResponse);

			// Send StartGameResponse to all users in the room
			m_communicator->sendMessageToUsers(usersInRoom, static_cast<int>(ResponseCode::START_GAME_RESPONSE), startGameResponseBuffer);

			std::cout << "Sent StartGameResponse to all " << usersInRoom.size() << " users in room " << roomId << std::endl;
		}

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult RoomAdminRequestHandler::handleGetRoomStateRequest(const RequestInfo& requestInfo)
{
	try
	{
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_username);

		if (!roomIdOpt.has_value())
		{
			return createErrorResponse("User is not in any room");
		}

		unsigned int roomId = roomIdOpt.value();

		// Get room object to access its properties
		std::optional<std::reference_wrapper<Room>> roomOpt = m_roomManager->getRoom(roomId);
		if (!roomOpt.has_value())
		{
			return createErrorResponse("Room not found");
		}

		Room& room = roomOpt->get();

		RoomState roomState = m_roomManager->getRoomState(roomId);

		std::vector<std::string> usersInRoom = room.getAllUsers();

		GetRoomStateResponse response;
		response.status = (unsigned int)Status::SUCCESS;
		response.players = usersInRoom;
		response.hasGameBegun = (roomState == RoomState::GAME_IN_PROGRESS);
		response.questionCount = room.getNumOfQuestionsInGame();
		response.answerTimeout = room.getTimePerQuestion();

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