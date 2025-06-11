#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "IRequestHandler.h"
#include "SqliteDataBase.h"

class LoggedUser
{
public:
	LoggedUser(const std::string& username);
	~LoggedUser() = default;

	std::string getUsername() const;
	void setUsername(std::string username);
private:
	std::string m_username;
};