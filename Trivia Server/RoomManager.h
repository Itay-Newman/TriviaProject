#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Room.h"
#include "Structs.h"
#include <optional>
#include <functional>

enum class RoomState
{
	WAITING_FOR_PLAYERS,
	GAME_IN_PROGRESS,
	CLOSED
};

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	// Room management functions
	unsigned int createRoom(const std::string& creator, const std::string& roomName, unsigned int maxPlayers,
	unsigned int numOfQuestionsInGame, unsigned int timePerQuestion);
	bool deleteRoom(unsigned int roomId);
	RoomState getRoomState(unsigned int roomId) const;
	std::vector<RoomData> getRooms() const;

	// Room access functions
	std::optional<std::reference_wrapper<Room>> getRoom(unsigned int roomId);
	bool addUserToRoom(unsigned int roomId, const std::string& username);
	bool removeUserFromRoom(unsigned int roomId, const std::string& username);
	std::vector<std::string> getUsersInRoom(unsigned int roomId) const;
	bool isRoomActive(unsigned int roomId) const;

private:
	unsigned int m_nextRoomId;
	std::unordered_map<unsigned int, Room> m_rooms;
};