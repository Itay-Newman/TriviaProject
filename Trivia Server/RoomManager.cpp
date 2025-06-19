#include "RoomManager.h"
#include "Room.h"
#include <iostream>

RoomManager::RoomManager() : m_nextRoomId(1)
{
}

RoomManager::~RoomManager()
{
	this->m_rooms.clear();
}

unsigned int RoomManager::createRoom(const std::string& creator, const std::string& roomName, unsigned int maxPlayers,
	unsigned int numOfQuestionsInGame, unsigned int timePerQuestion)
{
	Room room(this->m_nextRoomId, roomName, maxPlayers, numOfQuestionsInGame, timePerQuestion, creator);
	this->m_rooms.insert({ this->m_nextRoomId, room });

	unsigned int currentRoomId = this->m_nextRoomId;
	this->m_nextRoomId++;

	std::cout << "Room " << roomName << " created with ID " << currentRoomId << " by " << creator << std::endl;
	return currentRoomId;
}

bool RoomManager::deleteRoom(unsigned int roomId)
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		this->m_rooms.erase(it);
		std::cout << "Room " << roomId << " deleted" << std::endl;
		return true;
	}

	std::cout << "Room " << roomId << " not found" << std::endl;
	return false;
}

RoomState RoomManager::getRoomState(unsigned int roomId) const
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		const Room& room = it->second;

		if (room.getRoomState() == RoomState::GAME_IN_PROGRESS)
		{
			return RoomState::GAME_IN_PROGRESS;
		}
		else if (room.getRoomState() == RoomState::WAITING_FOR_PLAYERS)
		{
			return RoomState::WAITING_FOR_PLAYERS;
		}
		else if (room.getRoomState() == RoomState::CLOSED)
		{
			return RoomState::CLOSED;
		}
	}
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> roomsData;

	for (const auto& pair : this->m_rooms)
	{
		const Room& room = pair.second;
		if (room.getRoomState() == RoomState::GAME_IN_PROGRESS || room.getRoomState() == RoomState::WAITING_FOR_PLAYERS)
		{
			RoomData data;
			data.id = room.getId();
			data.name = room.getName();
			data.maxPlayers = room.getMaxPlayers();
			data.numOfQuestionsInGame = room.getNumOfQuestionsInGame();
			data.timePerQuestion = room.getTimePerQuestion();
			data.isActive = room.getRoomState() == RoomState::GAME_IN_PROGRESS || room.getRoomState() == RoomState::WAITING_FOR_PLAYERS;

			roomsData.push_back(data);
		}
	}

	return roomsData;
}

std::optional<std::reference_wrapper<Room>> RoomManager::getRoom(unsigned int roomId)
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		return it->second;
	}

	return std::nullopt;
}

bool RoomManager::addUserToRoom(unsigned int roomId, const std::string& username)
{
	auto maybeRoom = this->getRoom(roomId);
	if (maybeRoom)
	{
		return maybeRoom->get().addUser(username);
	}

	std::cout << "Room " << roomId << " not found" << std::endl;
	return false;
}

bool RoomManager::removeUserFromRoom(unsigned int roomId, const std::string& username)
{
	auto maybeRoom = this->getRoom(roomId);
	if (maybeRoom)
	{
		return maybeRoom->get().removeUser(username);
	}

	std::cout << "Room " << roomId << " not found" << std::endl;
	return false;
}

std::vector<std::string> RoomManager::getUsersInRoom(unsigned int roomId) const
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		return it->second.getAllUsers();
	}

	return {};
}

bool RoomManager::isRoomActive(unsigned int roomId) const
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		return it->second.getRoomState() == RoomState::WAITING_FOR_PLAYERS || it->second.getRoomState() == RoomState::GAME_IN_PROGRESS;
	}

	return false;
}

std::optional<unsigned int> RoomManager::getRoomIdByUser(const std::string& username) const
{
	for (const auto& pair : this->m_rooms)
	{
		const Room& room = pair.second;
		std::vector<std::string> users = room.getAllUsers();

		for (const std::string& user : users)
		{
			if (user == username)
			{
				return room.getId();
			}
		}
	}

	return std::nullopt; // User not found in any room
}

void RoomManager::setRoomState(unsigned int roomId, RoomState state)
{
	auto it = this->m_rooms.find(roomId);
	if (it != this->m_rooms.end())
	{
		it->second.setState(state);
		std::cout << "Room " << roomId << " state set to " << static_cast<int>(state) << std::endl;
	}
	else
	{
		std::cout << "Room " << roomId << " not found" << std::endl;
	}
}
