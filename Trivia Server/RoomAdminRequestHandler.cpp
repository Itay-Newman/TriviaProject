#include "BaseRoomRequestHandler.h"
#include "MenuRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager,
	const std::string& username, RequestHandlerFactory* handlerFactory) : BaseRoomRequestHandler(database, roomManager, statisticsManager, handlerFactory)
{
	this->m_username = username;
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;
	return code == static_cast<unsigned int>(RequestCodes::CLOSE_ROOM_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::START_GAME_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::GET_ROOM_STATE_REQUEST);
	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;

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

		this->m_roomManager->setRoomState(roomId, RoomState::CLOSED);

		bool success = m_roomManager->deleteRoom(roomId);

		CloseRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::CLOSE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = dynamic_cast<IRequestHandler*>(m_HandlerFactory.createMenuRequestHandler(m_username));

		//// Send LeaveRoomResponse to all room members
		//if (success)
		//{
		//	LeaveRoomResponse leaveResponse;
		//	leaveResponse.status = (unsigned int)Status::SUCCESS;
		//	std::vector<unsigned char> leaveResponseBuffer = JsonResponsePacketSerializer::serializeResponse(leaveResponse);

		//	// Send LeaveRoomResponse to all users in the room
		//	if (this->m_HandlerFactory.getCommunicator() != nullptr)
		//	{
		//		this->m_HandlerFactory.getCommunicator()->sendMessageToUsers(usersInRoom, static_cast<int>(ResponseCode::LEAVE_ROOM_RESPONSE), leaveResponseBuffer);
		//		std::cout << "Sent LeaveRoomResponse to all " << usersInRoom.size() << " users in room " << roomId << std::endl;
		//	}
		//}

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

		m_roomManager->setRoomState(roomId, RoomState::GAME_IN_PROGRESS);

		StartGameResponse response;
		response.status = (unsigned int)Status::SUCCESS;

		RequestResult result;
		result.id = ResponseCode::START_GAME_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this->m_HandlerFactory.createGameRequestHandler(this->m_username); // Will be replaced with a GameRequestHandler in the future

		//// Send StartGameResponse to all room members
		//if (this->m_HandlerFactory.getCommunicator() != nullptr)
		//{
		//	StartGameResponse startGameResponse;
		//	startGameResponse.status = (unsigned int)Status::SUCCESS;
		//	std::vector<unsigned char> startGameResponseBuffer = JsonResponsePacketSerializer::serializeResponse(startGameResponse);

		//	// Send StartGameResponse to all users in the room
		//	this->m_HandlerFactory.getCommunicator()->sendMessageToUsers(usersInRoom, static_cast<int>(ResponseCode::START_GAME_RESPONSE), startGameResponseBuffer);

		//	std::cout << "Sent StartGameResponse to all " << usersInRoom.size() << " users in room " << roomId << std::endl;
		//}

		//this->m_roomManager->deleteRoom(roomId);
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