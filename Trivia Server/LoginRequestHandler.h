#pragma once

#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory, IDatabase& database);
	~LoginRequestHandler() override = default;

	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestInfo handleRequest(const RequestInfo& requestInfo) override;
private:
	LoginManager& m_LoginManager;
	RequestHandlerFactory& m_HandlerFactory;
	IDatabase& m_database;
};
