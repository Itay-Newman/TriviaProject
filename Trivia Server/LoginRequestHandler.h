#pragma once

#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "loginManager.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(IRequestHandler* database, LoginManager& loginManager);
	~LoginRequestHandler();

	// Virtual functions implementation  
	virtual bool isRequestRelevant(RequestInfo requestInfo) override;
	virtual RequestInfo handleRequest(RequestInfo requestInfo) override;

	// Implementing pure virtual functions from IRequestHandler  
	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	IRequestHandler* m_Database;
	LoginManager& m_LoginManager;
};