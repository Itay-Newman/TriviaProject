#pragma once  
#include <string>  
#include <iostream>  
#include <vector>  
#include "IRequestHandler.h"  
#include "SqliteDataBase.h"  
#include "LoggedUser.h"  
#include "IDatabase.h"  

class LoginManager
{
public:
	LoginManager(IDatabase* database);
	~LoginManager() = default;
	int signUp(const std::string& username, const std::string& password, const std::string& email) const;
	int signIn(const std::string& username, const std::string& password);
	void signOut(const std::string& username);
private:
	std::vector<LoggedUser> loggedUsers;
	IRequestHandler* requestHandler;
	IDatabase* m_Database;
};