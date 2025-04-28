#include "loginManager.h"

int LoginManager::signUp(const std::string& username, const std::string& password, const std::string& email) const
{
	if (this->requestHandler->doesUserExist(username))
	{
		std::cout << "User already exists" << std::endl;
		return -1; // Means that the user already exists
	}

	if (this->requestHandler->addUser(username, password, email))
	{
		std::cout << "User added successfully" << std::endl;
		return 0; // Means that the user added successfully
	}
	else
	{
		std::cout << "Failed to add user" << std::endl;
		return -2; // Means that the failed to add user
	}
}

int LoginManager::signIn(const std::string& username, const std::string& password)
{
	if (this->requestHandler->doesUserExist(username))
	{
		if (this->requestHandler->doesPasswordMatch(username, password))
		{
			LoggedUser loggedUser(username);

			std::cout << "User signed in successfully" << std::endl;
			this->loggedUsers.push_back(loggedUser);
			return 0; // Means that the user signed in successfully
		}
		else
		{
			std::cout << "Incorrect password" << std::endl;
			return -1; // Means incorrect password
		}
	}
	else
	{
		std::cout << "User does not exist" << std::endl;
		return -2; // Means that the user does not exist
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
