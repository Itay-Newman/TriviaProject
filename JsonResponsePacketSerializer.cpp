#include "JsonResponsePacketSerializer.h"

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
	nlohmann::json j = {
		{"status", response.message}
	};

	std::string jsonString = j.dump();

	std::vector<unsigned char> jsonBytes(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	std::vector<unsigned char> jsonBytes(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	std::vector<unsigned char> jsonBytes(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	std::vector<unsigned char> jsonBytes(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"rooms", response.rooms}
	};

	std::string jsonString = j.dump();

	std::vector<unsigned char> jsonBytes(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& response)
{
	return std::vector<unsigned char>();
}
