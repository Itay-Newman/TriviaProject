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
	signupRequest.username = j.at("username");
	signupRequest.password = j.at("password");
	signupRequest.email = j.at("email");

	return signupRequest;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer) const
{
	GetPlayersInRoomRequest getPlayersRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	getPlayersRequest.roomId = j.at("roomId");

	return getPlayersRequest;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer) const
{
	JoinRoomRequest joinRoomRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	joinRoomRequest.roomId = j.at("roomId");

	return joinRoomRequest;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer) const
{
	CreateRoomRequest createRoomRequest;

	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	createRoomRequest.roomName = j.at("roomName");
	createRoomRequest.maxUsers = j.at("maxUsers");
	createRoomRequest.questionCount = j.at("questionCount");
	createRoomRequest.answerTimeout = j.at("answerTimeout");

	return createRoomRequest;
}
