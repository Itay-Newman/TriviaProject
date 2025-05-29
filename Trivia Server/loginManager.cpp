#include "loginManager.h"

LoginManager::LoginManager(IDatabase& database) : m_Database(database)
{
}

int LoginManager::signUp(const std::string& username, const std::string& password, const std::string& email) const
{
	if (this->m_Database.doesUserExist(username))
	{
		std::cout << "User already exists" << std::endl;
		return static_cast<int>(Status::FAILURE);
	}

	if (this->m_Database.addUser(username, password, email))
	{
		std::cout << "User added successfully" << std::endl;
		return static_cast<int>(Status::SUCCESS);
	}
	else
	{
		std::cout << "Failed to add user" << std::endl;
		return static_cast<int>(Status::FAILURE);
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
			return static_cast<int>(Status::SUCCESS);
		}
		else
		{
			std::cout << "Incorrect password" << std::endl;
			return static_cast<int>(Status::FAILURE);
		}
	}
	else
	{
		std::cout << "User does not exist" << std::endl;
		return static_cast<int>(Status::FAILURE);
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
