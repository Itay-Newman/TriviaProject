#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
	LoginRequest loginRequest;

	// Parse the JSON from the buffer
	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	loginRequest.username = j.at("username");
	loginRequest.password = j.at("password");

	return loginRequest;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char>& buffer)
{
	SignupRequest signupRequest;

	// Parse the JSON from the buffer
	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	// Extract fields from JSON to the struct
	signupRequest.username = j["username"];
	signupRequest.password = j["password"];
	signupRequest.email = j["email"];

	return signupRequest;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer) const
{
	GetPlayersInRoomRequest getPlayersRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	getPlayersRequest.roomId = j["roomId"];

	return getPlayersRequest;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer) const
{
	JoinRoomRequest joinRoomRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	joinRoomRequest.roomId = j["roomId"];

	return joinRoomRequest;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer) const
{
	CreateRoomRequest createRoomRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	createRoomRequest.roomName = j["roomName"];
	createRoomRequest.maxUsers = j["maxUsers"];
	createRoomRequest.questionCount = j["questionCount"];
	createRoomRequest.answerTimeout = j["answerTimeout"];

	return createRoomRequest;
}
