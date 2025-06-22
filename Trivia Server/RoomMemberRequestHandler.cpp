#include "BaseRoomRequestHandler.h"
#include "MenuRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager,
	const std::string& username, RequestHandlerFactory* handlerFactory)
	: BaseRoomRequestHandler(database, roomManager, statisticsManager, handlerFactory)
{
	this->m_username = username;
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;
	return code == static_cast<unsigned int>(RequestCodes::LEAVE_ROOM_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::GET_ROOM_STATE_REQUEST);
	return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;

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

RequestResult RoomMemberRequestHandler::handleLeaveRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_username);

		if (!roomIdOpt.has_value())
		{
			return createErrorResponse("User is not in any room");
		}

		unsigned int roomId = roomIdOpt.value();

		// Get all users in the room BEFORE removing the user
		std::vector<std::string> usersInRoom = m_roomManager->getUsersInRoom(roomId);

		bool success = m_roomManager->removeUserFromRoom(roomId, m_username);

		LeaveRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::LEAVE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = (m_HandlerFactory.createMenuRequestHandler(m_username));

		// Send LeaveRoomResponse to all remaining room members (excluding the user who left)
		if (success && this->m_HandlerFactory.getCommunicator() != nullptr)
		{
			// Remove the leaving user from the list
			usersInRoom.erase(std::remove(usersInRoom.begin(), usersInRoom.end(), m_username), usersInRoom.end());

			if (!usersInRoom.empty())
			{
				LeaveRoomResponse leaveResponse;
				leaveResponse.status = (unsigned int)Status::SUCCESS;
				std::vector<unsigned char> leaveResponseBuffer = JsonResponsePacketSerializer::serializeResponse(leaveResponse);

				// Send LeaveRoomResponse to all remaining users in the room
				this->m_HandlerFactory.getCommunicator()->sendMessageToUsers(usersInRoom, static_cast<int>(ResponseCode::LEAVE_ROOM_RESPONSE), leaveResponseBuffer);

				std::cout << "Sent LeaveRoomResponse to all " << usersInRoom.size() << " remaining users in room " << roomId << std::endl;
			}
		}

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult RoomMemberRequestHandler::handleGetRoomStateRequest(const RequestInfo& requestInfo)
{
	try
	{
		GetRoomStateResponse response{};
		RequestResult result{};

		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_username);

		if (!roomIdOpt.has_value())
		{
			response.status = (unsigned int)Status::FAILURE;
			response.hasGameBegun = 0;
			response.players = std::vector<std::string>();
			response.questionCount = 0;
			response.answerTimeout = 0;

			result.id = ResponseCode::GET_ROOM_STATE_RESPONSE;
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = this->m_HandlerFactory.createMenuRequestHandler(this->m_username);

			return result;
		}
		else
		{
			unsigned int roomId = roomIdOpt.value();

			std::optional<std::reference_wrapper<Room>> roomOpt = m_roomManager->getRoom(roomId);

			Room& room = roomOpt->get();

			RoomState roomState = m_roomManager->getRoomState(roomId);

			std::vector<std::string> usersInRoom = room.getAllUsers();

			response.status = (unsigned int)Status::SUCCESS;
			response.hasGameBegun = (roomState == RoomState::GAME_IN_PROGRESS);
			response.players = usersInRoom;
			response.questionCount = room.getNumOfQuestionsInGame();
			response.answerTimeout = room.getTimePerQuestion();

			result.id = ResponseCode::GET_ROOM_STATE_RESPONSE;
			result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = this;
			if (roomState == RoomState::GAME_IN_PROGRESS)
			{
				result.newHandler = m_HandlerFactory.createGameRequestHandler(this->m_username);
			}
		}
		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}
