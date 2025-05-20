#pragma once
#include <string>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& name) const = 0;
	virtual bool doesPasswordMatch(const std::string& name, const std::string& password) const = 0;
	virtual bool addUser(const std::string& name, const std::string& password, const std::string& email) const = 0;
};