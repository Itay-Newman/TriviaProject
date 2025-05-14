#include "RoomManager.h"

RoomManager::RoomManager() : m_nextRoomId(1)
{
}

RoomManager::~RoomManager()
{
    // Clean up all rooms
    for (auto& room : m_rooms)
    {
        delete room.second;
    }
    m_rooms.clear();
}

unsigned int RoomManager::createRoom(const std::string& creator, const std::string& roomName, unsigned int maxPlayers,
    unsigned int numOfQuestionsInGame, unsigned int timePerQuestion)
{
    // Create a new room
    Room* room = new Room(m_nextRoomId, roomName, maxPlayers, numOfQuestionsInGame, timePerQuestion, creator);

    // Add the room to the map
    m_rooms[m_nextRoomId] = room;

    // Increment the next room ID
    unsigned int currentRoomId = m_nextRoomId;
    m_nextRoomId++;

    std::cout << "Room " << roomName << " created with ID " << currentRoomId << " by " << creator << std::endl;
    return currentRoomId;
}

bool RoomManager::deleteRoom(unsigned int roomId)
{
    // Find the room
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
    {
        // Delete the room
        delete it->second;
        m_rooms.erase(it);
        std::cout << "Room " << roomId << " deleted" << std::endl;
        return true;
    }

    std::cout << "Room " << roomId << " not found" << std::endl;
    return false;
}

RoomState RoomManager::getRoomState(unsigned int roomId) const
{
    // Find the room
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
    {
        Room* room = it->second;

        // Check if the room is active
        if (!room->getIsActive())
        {
            return RoomState::CLOSED;
        }

        // For now, we'll assume that if the room is active and has users, it's in the waiting state
        // Later, you can add game state checks here
        return RoomState::WAITING_FOR_PLAYERS;
    }

    // Room not found, return closed state
    return RoomState::CLOSED;
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> roomsData;

    // Convert all active rooms to RoomData structs
    for (const auto& pair : m_rooms)
    {
        Room* room = pair.second;

        // Only include active rooms
        if (room->getIsActive())
        {
            RoomData data;
            data.id = room->getId();
            data.name = room->getName();
            data.maxPlayers = room->getMaxPlayers();
            data.numOfQuestionsInGame = room->getNumOfQuestionsInGame();
            data.timePerQuestion = room->getTimePerQuestion();
            data.isActive = room->getIsActive() ? 1 : 0;

            roomsData.push_back(data);
        }
    }

    return roomsData;
}

Room* RoomManager::getRoom(unsigned int roomId)
{
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
    {
        return it->second;
    }

    return nullptr;
}

bool RoomManager::addUserToRoom(unsigned int roomId, const std::string& username)
{
    Room* room = getRoom(roomId);
    if (room)
    {
        return room->addUser(username);
    }

    std::cout << "Room " << roomId << " not found" << std::endl;
    return false;
}

bool RoomManager::removeUserFromRoom(unsigned int roomId, const std::string& username)
{
    Room* room = getRoom(roomId);
    if (room)
    {
        return room->removeUser(username);
    }

    std::cout << "Room " << roomId << " not found" << std::endl;
    return false;
}

std::vector<std::string> RoomManager::getUsersInRoom(unsigned int roomId) const
{
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
    {
        return it->second->getAllUsers();
    }

    // Return empty vector if room not found
    return std::vector<std::string>();
}

bool RoomManager::isRoomActive(unsigned int roomId) const
{
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
    {
        return it->second->getIsActive();
    }

    // Room not found, return false
    return false;
}