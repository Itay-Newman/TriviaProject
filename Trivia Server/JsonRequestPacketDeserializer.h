#pragma once

#include <vector>
#include <string>
#include "Structs.h"
#include "json.hpp"

using nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
	static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const std::vector<unsigned char>& buffer);

private:
	static json parseJson(const std::vector<unsigned char>& buffer);
};