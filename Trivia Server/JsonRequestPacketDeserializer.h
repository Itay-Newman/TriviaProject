#pragma once

#include <vector>
#include <chrono>
#include "Structs.h"
#include "json.hpp"
using nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
	static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
	GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer) const;
	JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer) const;
	CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer) const;
};