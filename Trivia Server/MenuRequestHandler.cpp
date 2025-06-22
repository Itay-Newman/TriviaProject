#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

MenuRequestHandler::MenuRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager, const std::string& username, RequestHandlerFactory& handlerFactory)
	: m_database(database), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_handlerFactory(handlerFactory), m_user(username)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	int code = requestInfo.id;
	return code == static_cast<unsigned char>(RequestCodes::CREATE_ROOM_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::GET_ROOMS_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::GET_PLAYERS_IN_ROOM_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::JOIN_ROOM_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::GET_PERSONAL_STATS_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::LOGOUT_REQUEST) ||
		code == static_cast<unsigned char>(RequestCodes::GET_HIGH_SCORES_REQUEST);
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	unsigned char code = requestInfo.id;

	switch (static_cast<RequestCodes>(code))
	{
	case RequestCodes::CREATE_ROOM_REQUEST:
		return handleCreateRoomRequest(requestInfo);
	case RequestCodes::GET_ROOMS_REQUEST:
		return handleGetRoomsRequest(requestInfo);
	case RequestCodes::GET_PLAYERS_IN_ROOM_REQUEST:
		return handleGetPlayersInRoomRequest(requestInfo);
	case RequestCodes::JOIN_ROOM_REQUEST:
		return handleJoinRoomRequest(requestInfo);
	case RequestCodes::GET_PERSONAL_STATS_REQUEST:
		return handleGetPersonalStatisticsRequest(requestInfo);
	case RequestCodes::GET_HIGH_SCORES_REQUEST:
		return handleGetHighScoresRequest(requestInfo);
	case RequestCodes::LOGOUT_REQUEST:
		return handleLogoutRequest(requestInfo);
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

	ErrorResponse errorResponse;
	errorResponse.status = ResponseCode::ERROR_RESPONSE;
	errorResponse.message = "Invalid request format";

	RequestResult result;
	result.id = ResponseCode::ERROR_RESPONSE;
	result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
	result.newHandler = this;
	return result;
}

bool MenuRequestHandler::doesUserExist(const std::string& username) const
{
	// Use database to check if user exists
	return m_database.doesUserExist(username);
}

bool MenuRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	// Use database to check password
	return m_database.doesPasswordMatch(username, password);
}

bool MenuRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	// Use database to add user
	return m_database.addUser(username, password, email);
}

RequestResult MenuRequestHandler::handleCreateRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		// Deserialize the create room request
		JsonRequestPacketDeserializer deserializer;
		CreateRoomRequest createRoomRequest = deserializer.deserializeCreateRoomRequest(requestInfo.buffer);

		// Create the room using the room manager
		unsigned int roomId = m_roomManager->createRoom(
			m_user.getUsername(),
			createRoomRequest.RoomName,
			createRoomRequest.MaxUsers,
			createRoomRequest.QuestionCount,
			createRoomRequest.AnswerTimeout
		);

		// Prepare the response
		CreateRoomResponse response;
		response.status = (unsigned int)Status::SUCCESS;

		// Serialize the response
		RequestResult result;
		result.id = ResponseCode::CREATE_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user.getUsername());

		return result;
	}
	catch (const std::exception& e)
	{
		// Handle error
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

RequestResult MenuRequestHandler::handleGetRoomsRequest(const RequestInfo& requestInfo)
{
	try
	{
		// Get all rooms from the room manager
		std::vector<RoomData> rooms = m_roomManager->getRooms();

		GetRoomsResponse response;
		response.status = (unsigned int)Status::SUCCESS;
		response.rooms = rooms;

		RequestResult result;
		result.id = ResponseCode::GET_ROOMS_RESPONSE;
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

RequestResult MenuRequestHandler::handleGetPlayersInRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		// Deserialize the get players in room request
		JsonRequestPacketDeserializer deserializer;
		GetPlayersInRoomRequest request = deserializer.deserializeGetPlayersInRoomRequest(requestInfo.buffer);

		// Get the users in the room
		std::vector<std::string> users = m_roomManager->getUsersInRoom(request.roomId);

		// Prepare the response
		GetPlayersInRoomResponse response;
		response.users = users;

		// Serialize the response
		RequestResult result;
		result.id = ResponseCode::GET_PLAYERS_IN_ROOM_RESPONSE;
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

RequestResult MenuRequestHandler::handleJoinRoomRequest(const RequestInfo& requestInfo)
{
	try
	{
		// Deserialize the join room request
		JsonRequestPacketDeserializer deserializer;
		JoinRoomRequest request = deserializer.deserializeJoinRoomRequest(requestInfo.buffer);

		// Add the user to the room
		bool success = m_roomManager->addUserToRoom(request.roomId, m_user.getUsername());

		// Prepare the response
		JoinRoomResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		// Serialize the response
		RequestResult result;
		result.id = ResponseCode::JOIN_ROOM_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user.getUsername());

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

RequestResult MenuRequestHandler::handleGetPersonalStatisticsRequest(const RequestInfo& requestInfo)
{
	try
	{
		PlayerStatistics playerStats = m_statisticsManager->getPlayerStatistics(m_user.getUsername());

		std::vector<std::string> statistics;

		statistics.push_back("Username: " + playerStats.username);
		statistics.push_back("Average Answer Time: " + std::to_string(playerStats.averageAnswerTime));
		statistics.push_back("Correct Answers: " + std::to_string(playerStats.correctAnswers));
		statistics.push_back("Wrong Answers: " + std::to_string(playerStats.wrongAnswers));
		statistics.push_back("Total Games: " + std::to_string(playerStats.totalGames));
		statistics.push_back("Score: " + std::to_string(playerStats.score));

		GetPersonalStatsResponse response;
		response.status = (unsigned int)Status::SUCCESS;
		response.statistics = statistics;

		RequestResult result;
		result.id = ResponseCode::GET_PERSONAL_STATS_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		ErrorResponse errorResponse;
		errorResponse.status = ResponseCode::ERROR_RESPONSE;
		errorResponse.message = e.what();

		RequestResult result
		{
			.id = ResponseCode::ERROR_RESPONSE,
			.response = JsonResponsePacketSerializer::serializeResponse(errorResponse),
			.newHandler = this,
		};

		return result;
	}
}

RequestResult MenuRequestHandler::handleGetHighScoresRequest(const RequestInfo& requestInfo)
{
	try
	{
		std::vector<PlayerStatistics> highScores = m_statisticsManager->getHighScores();

		std::vector<std::string> statistics;
		statistics.push_back("High Scores:");

		for (size_t i = 0; i < highScores.size(); ++i)
		{
			statistics.push_back(std::to_string(i + 1) + ". " +
				highScores[i].username + " - " +
				std::to_string(highScores[i].score));
		}

		GetHighScoreResponse response
		{
			.status = (unsigned int)Status::SUCCESS,
			.statistics = statistics
		};

		RequestResult result
		{
			.id = ResponseCode::GET_HIGH_SCORE_RESPONSE,
			.response = JsonResponsePacketSerializer::serializeResponse(response),
			.newHandler = this
		};

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

RequestResult MenuRequestHandler::handleLogoutRequest(const RequestInfo& requestInfo)
{
	try
	{
		// Log out the user using LoginManager's signOut method
		this->m_handlerFactory.getLoginManager().signOut(m_user.getUsername());

		// Prepare the response
		LogoutResponse response;
		response.status = (unsigned int)Status::SUCCESS; // Assuming success since logOut doesn't return a status

		// Serialize the response
		RequestResult result;
		result.id = ResponseCode::LOGOUT_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = nullptr; // User is logging out, so no handler is needed

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