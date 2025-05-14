#include "loginManager.h"

#define LOGIN_SUCCESS 1
#define LOGIN_FAILURE -1
#define SIGNUP_SUCCESS 1
#define USER_ALREADY_EXISTS -1
#define SIGNUP_FAILURE -2
#define USER_NOT_FOUND -2

LoginManager::LoginManager(IDatabase& database) : m_Database(database)
{
}

int LoginManager::signUp(const std::string& username, const std::string& password, const std::string& email) const
{
	if (this->m_Database.doesUserExist(username))
	{
		std::cout << "User already exists" << std::endl;
		return USER_ALREADY_EXISTS;
	}
	
	if (this->m_Database.addUser(username, password, email))
	{
		std::cout << "User added successfully" << std::endl;
		return SIGNUP_SUCCESS;
	}
	else
	{
		std::cout << "Failed to add user" << std::endl;
		return SIGNUP_FAILURE;
	}
}

int LoginManager::signIn(const std::string& username, const std::string& password)
{
	if (this->m_Database.doesUserExist(username))
	{
		if (this->m_Database.doesPasswordMatch(username, password))
		{
			LoggedUser loggedUser(username);

			std::cout << "User signed in successfully" << std::endl;
			this->loggedUsers.push_back(loggedUser);
			return LOGIN_SUCCESS; //! User signed in successfully
		}
		else
		{
			std::cout << "Incorrect password" << std::endl;
			return LOGIN_FAILURE; //! Incorrect password
		}
	}
	else
	{
		std::cout << "User does not exist" << std::endl;
		return USER_NOT_FOUND; //! User does not exist
	}
}

void LoginManager::signOut(const std::string& username)
{
	auto userIt = std::find_if(
		this->loggedUsers.begin(),
		this->loggedUsers.end(),
		[&](const LoggedUser& user)
		{
			return user.getUsername() == username;
		});

	if (userIt != this->loggedUsers.end())
	{
		this->loggedUsers.erase(userIt);
		std::cout << "User signed out successfully" << std::endl;
	}
	else
	{
		std::cout << "User not found" << std::endl;
	}
}
