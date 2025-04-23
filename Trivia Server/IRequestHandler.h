#pragma once

#include "Structs.h"

class IRequestHandler
{
public:
	IRequestHandler() = default;
	virtual ~IRequestHandler() = default;
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const = 0;
	virtual bool isRequestRelevant(RequestInfo requestInfo) = 0;
	virtual RequestInfo handleRequest(RequestInfo requestInfo) = 0;
};