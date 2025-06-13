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
		j.at("RoomId").get<unsigned int>()
	};
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return CreateRoomRequest{
		j.at("RoomName").get<std::string>(),
		j.at("MaxUsers").get<unsigned int>(),
		j.at("QuestionCount").get<unsigned int>(),
		j.at("AnswerTimeout").get<unsigned int>()
	};
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const std::vector<unsigned char>& buffer)
{
	auto j = parseJson(buffer);

	return SubmitAnswerRequest{
		j.at("answerId").get<unsigned int>(),
		j.at("answerTime").get<unsigned int>()
	};
}

bool JsonRequestPacketDeserializer::isRequestWithNoData(unsigned int requestCode)
{
	// List of request codes that don't need parsing of extra data
	switch (requestCode)
	{
	case 6:  // GET_HIGH_SCORE_REQUEST
	case 29: // LOGOUT_REQUEST
	case 22: // GET_ROOMS_REQUEST
	case 26: // CLOSE_ROOM_REQUEST
	case 31: // START_GAME_REQUEST
	case 27: // GET_ROOM_STATE_REQUEST
	case 28: // LEAVE_ROOM_REQUEST
	case 32: // LEAVE_GAME_REQUEST
	case 33: // GET_QUESTION_REQUEST
	case 35: // GET_GAME_RESULTS_REQUEST
		return true;
	default:
		return false;
	}
}