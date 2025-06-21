#pragma once

#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "Structs.h"
#include "MenuRequestHandler.h"

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
	RequestResult handleRequest(const RequestInfo& requestInfo) override;

	std::string getName() const override { return "LoginRequestHandler"; }

private:
	LoginManager& m_loginManager;
	RequestHandlerFactory& m_handlerFactory;
	IDatabase& m_database;
};
