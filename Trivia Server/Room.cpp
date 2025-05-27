#include "Room.h"
#include <algorithm>

Room::Room(unsigned int id, const std::string& name, unsigned int maxPlayers, unsigned int numOfQuestionsInGame,
	unsigned int timePerQuestion, const std::string& creator)
	: m_id(id), m_name(name), m_maxPlayers(maxPlayers), m_numOfQuestionsInGame(numOfQuestionsInGame),
	m_timePerQuestion(timePerQuestion), m_isActive(true)
{
	// Adding the creator as the first user in the room
	this->m_users.push_back(creator);
}

unsigned int Room::getId() const
{
	return this->m_id;
}

std::string Room::getName() const
{
	return this->m_name;
}

unsigned int Room::getMaxPlayers() const
{
	return this->m_maxPlayers;
}

unsigned int Room::getNumOfQuestionsInGame() const
{
	return this->m_numOfQuestionsInGame;
}

unsigned int Room::getTimePerQuestion() const
{
	return this->m_timePerQuestion;
}

bool Room::getIsActive() const
{
	return this->m_isActive;
}

std::vector<std::string> Room::getAllUsers() const
{
	return this->m_users;
}

void Room::setIsActive(bool isActive)
{
	this->m_isActive = isActive;
}

bool Room::addUser(const std::string& username)
{
	// Check if the room is full
	if (this->m_users.size() >= this->m_maxPlayers)
	{
		std::cout << "Room is full" << std::endl;
		return false;
	}

	// Checking if the user is already in the room
	if (isUserInRoom(username))
	{
		std::cout << "User " << username << " is already in the room" << std::endl;
		return false;
	}

	// Adding the user to the room
	this->m_users.push_back(username);
	std::cout << "User " << username << " added to room " << this->m_name << std::endl;
	return true;
}

bool Room::removeUser(const std::string& username)
{
	// Finding the user in the room
	auto it = std::find(this->m_users.begin(), this->m_users.end(), username);
	if (it != this->m_users.end())
	{
		// Removing the user from the room
		this->m_users.erase(it);
		std::cout << "User " << username << " removed from room " << this->m_name << std::endl;
		return true;
	}

	std::cout << "User " << username << " not found in room " << this->m_name << std::endl;
	return false;
}

bool Room::isUserInRoom(const std::string& username) const
{
	return std::find(this->m_users.begin(), this->m_users.end(), username) != this->m_users.end();
}