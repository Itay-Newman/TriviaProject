#pragma once
#include <string>

class IDatabase
{
	virtual bool doesUserExist(const std::string& name) const;
	virtual bool doesPasswordMatch(const std::string& name, const std::string& password) const;
	virtual bool addUser(const std::string& name, const std::string& password, const std::string& email) const;
};