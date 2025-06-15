#include "GameRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "GameManager.h"

GameRequestHandler::GameRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager,
	GameManager* gameManager, const std::string& username, RequestHandlerFactory& handlerFactory)
	: m_database(database), m_roomManager(roomManager), m_statisticsManager(statisticsManager),
	m_gameManager(gameManager), m_handlerFactory(handlerFactory), m_user(username)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;
	return code == static_cast<unsigned int>(RequestCodes::LEAVE_GAME_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::GET_QUESTION_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::SUBMIT_ANSWER_REQUEST) ||
		code == static_cast<unsigned int>(RequestCodes::GET_GAME_RESULTS_REQUEST);
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	unsigned int code = requestInfo.id;

	switch (static_cast<RequestCodes>(code))
	{
	case RequestCodes::LEAVE_GAME_REQUEST:
		return handleLeaveGameRequest(requestInfo);
	case RequestCodes::GET_QUESTION_REQUEST:
		return handleGetQuestionRequest(requestInfo);
	case RequestCodes::SUBMIT_ANSWER_REQUEST:
		return handleSubmitAnswerRequest(requestInfo);
	case RequestCodes::GET_GAME_RESULTS_REQUEST:
		return handleGetGameResultsRequest(requestInfo);
	default:
		return createErrorResponse("Request type not supported");
	}
}

bool GameRequestHandler::doesUserExist(const std::string& username) const
{
	return m_database.doesUserExist(username);
}

bool GameRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_database.doesPasswordMatch(username, password);
}

bool GameRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_database.addUser(username, password, email);
}

RequestResult GameRequestHandler::handleLeaveGameRequest(const RequestInfo& requestInfo)
{
	try
	{
		std::optional<unsigned int> roomIdOpt = m_roomManager->getRoomIdByUser(m_user.getUsername());

		if (!roomIdOpt.has_value())
		{
			return createErrorResponse("User is not in any room");
		}

		unsigned int roomId = roomIdOpt.value();
		bool success = m_roomManager->removeUserFromRoom(roomId, m_user.getUsername());

		// Preparing the response
		LeaveGameResponse response;
		response.status = success ? (unsigned int)Status::SUCCESS : (unsigned int)Status::FAILURE;

		RequestResult result;
		result.id = ResponseCode::LEAVE_GAME_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult GameRequestHandler::handleGetQuestionRequest(const RequestInfo& requestInfo)
{
	try
	{
		GetQuestionResponse response = m_gameManager->getNextQuestion(m_user.getUsername());

		RequestResult result;
		result.id = ResponseCode::GET_QUESTION_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult GameRequestHandler::handleSubmitAnswerRequest(const RequestInfo& requestInfo)
{
	try
	{
		JsonRequestPacketDeserializer deserializer;
		SubmitAnswerRequest request = deserializer.deserializeSubmitAnswerRequest(requestInfo.buffer);

		SubmitAnswerResponse response = m_gameManager->submitAnswer(m_user.getUsername(), request.answerId, request.answerTime);

		RequestResult result;
		result.id = ResponseCode::SUBMIT_ANSWER_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult GameRequestHandler::handleGetGameResultsRequest(const RequestInfo& requestInfo)
{
	try
	{
		GetGameResultsResponse response = m_gameManager->getGameResults(m_user.getUsername());

		RequestResult result;
		result.id = ResponseCode::GET_GAME_RESULTS_RESPONSE;
		result.response = JsonResponsePacketSerializer::serializeResponse(response);
		result.newHandler = this;

		return result;
	}
	catch (const std::exception& e)
	{
		return createErrorResponse(e.what());
	}
}

RequestResult GameRequestHandler::createErrorResponse(const std::string& message)
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