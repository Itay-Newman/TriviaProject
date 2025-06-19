#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Structs.h"

class Room
{
public:
	Room(unsigned int id, const std::string& name, unsigned int maxPlayers, unsigned int numOfQuestionsInGame, unsigned int timePerQuestion, const std::string& creator);
	~Room() = default;

	// Getters
	unsigned int getId() const;
	std::string getName() const;
	unsigned int getMaxPlayers() const;
	unsigned int getNumOfQuestionsInGame() const;
	unsigned int getTimePerQuestion() const;
	RoomState getRoomState() const;
	std::vector<std::string> getAllUsers() const;

	// User management functions
	bool addUser(const std::string& username);
	bool removeUser(const std::string& username);
	bool isUserInRoom(const std::string& username) const;

	// Room state management functions
	void setState(RoomState roomState);

private:
	unsigned int m_id;
	std::string m_name;
	unsigned int m_maxPlayers;
	unsigned int m_numOfQuestionsInGame;
	unsigned int m_timePerQuestion;
	RoomState m_RoomState;
	std::vector<std::string> m_users;
};