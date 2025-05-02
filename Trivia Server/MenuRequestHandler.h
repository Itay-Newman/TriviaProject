#pragma once

#include "IRequestHandler.h"
#include "loginManager.h"

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(IRequestHandler* database, LoginManager& loginManager);
	~MenuRequestHandler();

	virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
	virtual RequestInfo handleRequest(const RequestInfo& requestInfo) override;

	virtual bool doesUserExist(const std::string& username) const override;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;
};