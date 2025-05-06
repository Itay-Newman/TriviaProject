#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
	LoginRequest loginRequest;

	// Parse the JSON from the buffer
	std::string jsonStr(buffer.begin(), buffer.end());
	nlohmann::json j = nlohmann::json::parse(jsonStr);

	// Extract fields from JSON to the struct
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