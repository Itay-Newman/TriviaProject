#pragma once
#include <string>
#include <vector>

class IDatabase
{
public:
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& email) const = 0;
};