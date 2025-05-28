#include "JsonRequestPacketDeserializer.h"

json JsonRequestPacketDeserializer::parseJson(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	return json::parse(jsonStr);
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return LoginRequest{
		j.at("Username").get<std::string>(),
		j.at("Password").get<std::string>()
	};
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return SignupRequest{
		j.at("Username").get<std::string>(),
		j.at("Password").get<std::string>(),
		j.at("Email").get<std::string>()
	};
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return GetPlayersInRoomRequest{
		j.at("roomId").get<unsigned int>()
	};
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return JoinRoomRequest{
		j.at("roomId").get<unsigned int>()
	};
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return CreateRoomRequest{
		j.at("roomName").get<std::string>(),
		j.at("maxUsers").get<unsigned int>(),
		j.at("questionCount").get<unsigned int>(),
		j.at("answerTimeout").get<unsigned int>()
	};
}

bool JsonRequestPacketDeserializer::isRequestWithNoData(unsigned int requestCode)
{
	// List request codes that don't need parsing of extra data
	switch (requestCode)
	{
	case 6:  // GET_HIGH_SCORE_REQUEST
	case 29: // LOGOUT_REQUEST
	case 22: // GET_ROOMS_REQUEST
		return true;
	default:
		return false;
	}
}
