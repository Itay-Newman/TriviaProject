#pragma once

#include <vector>
#include <string>
#include "Structs.h"
#include "json.hpp"

using nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	static bool isRequestWithNoData(unsigned int requestCode);

	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
	static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer);

	// These requests have no data, only request code
	static CloseRoomRequest deserializeCloseRoomRequest(const std::vector<unsigned char>& buffer);
	static GetRoomStateRequest deserializeGetRoomStateRequest(const std::vector<unsigned char>& buffer);
	static LeaveRoomRequest deserializeLeaveRoomRequest(const std::vector<unsigned char>& buffer);

private:
	static json parseJson(const std::vector<unsigned char>& buffer);
};