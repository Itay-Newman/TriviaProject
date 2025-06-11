#include "JsonResponsePacketSerializer.h"

std::vector<unsigned char> JsonResponsePacketSerializer::toBytes(const json& j)
{
	std::string jsonString = j.dump();
	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"message", response.message}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"rooms", nlohmann::json::array()}
	};

	for (const auto& room : response.rooms)
	{
		j["rooms"].push_back({
			{"id", room.id},
			{"name", room.name},
			{"maxPlayers", room.maxPlayers},
			{"numOfQuestionsInGame", room.numOfQuestionsInGame},
			{"timePerQuestion", room.timePerQuestion},
			{"isActive", room.isActive}
			});
	}

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status },
		{"players", response.users }
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"statistics", response.statistics}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"statistics", response.statistics}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"hasGameBegun", response.hasGameBegun},
		{"players", response.players},
		{"questionCount", response.questionCount},
		{"answerTimeout", response.answerTimeout}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"results", nlohmann::json::array()}
	};

	for (const auto& result : response.results)
	{
		j["results"].push_back({
			{"username", result.username},
			{"correctAnswerCount", result.correctAnswerCount},
			{"wrongAnswerCount", result.wrongAnswerCount},
			{"averageAnswerTime", result.averageAnswerTime}
			});
	}

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"correctAnswerId", response.correctAnswerId}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"question", response.question},
		{"answers", response.answers}
	};

	return toBytes(j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	return toBytes(j);
}

