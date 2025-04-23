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
private:
	std::string username;
};