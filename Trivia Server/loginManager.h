#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "IRequestHandler.h"
#include "SqliteDataBase.h"
#include "LoggedUser.h"

class LoginManager
{
public:
	LoginManager() = default;
	~LoginManager() = default;
	int signUp(const std::string& username, const std::string& password, const std::string& email) const;
	int signIn(const std::string& username, const std::string& password);
	void signOut(const std::string& username);
private:
	std::vector<LoggedUser> loggedUsers;
	IRequestHandler* requestHandler;
};