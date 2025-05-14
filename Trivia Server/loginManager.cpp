#include "loginManager.h"

LoginManager::LoginManager(IDatabase& database) : m_Database(database)
{
}

int LoginManager::signUp(const std::string& username, const std::string& password, const std::string& email) const
{
	if (this->m_Database.doesUserExist(username))
	{
		std::cout << "User already exists" << std::endl;
		return -1;
	}
	
	if (this->m_Database.addUser(username, password, email))
	{
		std::cout << "User added successfully" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Failed to add user" << std::endl;
		return -2;
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
			return 0; //! User signed in successfully
		}
		else
		{
			std::cout << "Incorrect password" << std::endl;
			return -1; //! Incorrect password
		}
	}
	else
	{
		std::cout << "User does not exist" << std::endl;
		return -2; //! User does not exist
	}
}

void LoginManager::signOut(const std::string& username)
{
	for (auto it = this->loggedUsers.begin(); it != this->loggedUsers.end(); ++it)
	{
		if (it->getUsername() == username)
		{
			this->loggedUsers.erase(it);
			std::cout << "User signed out successfully" << std::endl;
			return;
		}
	}
	std::cout << "User not found" << std::endl;
}
