#pragma once

#include "IRequestHandler.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "LoggedUser.h"

class RequestHandlerFactory;
class GameManager;

class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(IDatabase& database, RoomManager* roomManager, StatisticsManager* statisticsManager,
		GameManager* gameManager, const std::string& username, RequestHandlerFactory& handlerFactory);
	~GameRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	RequestResult handleLeaveGameRequest(const RequestInfo& requestInfo);
	RequestResult handleGetQuestionRequest(const RequestInfo& requestInfo);
	RequestResult handleSubmitAnswerRequest(const RequestInfo& requestInfo);
	RequestResult handleGetGameResultsRequest(const RequestInfo& requestInfo);
	RequestResult createErrorResponse(const std::string& message);

	std::string getName() const override { return "GameRequestHandler"; }

	IDatabase& m_database;
	RoomManager* m_roomManager;
	StatisticsManager* m_statisticsManager;
	GameManager* m_gameManager;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
};