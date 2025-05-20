#include "JsonResponsePacketSerializer.h"
#include <sstream>

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"message", response.message}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	std::stringstream roomsStream;

	for (size_t i = 0; i < response.rooms.size(); ++i)
	{
		roomsStream << response.rooms[i].name;
		if (i < response.rooms.size() - 1)
		{
			roomsStream << ", ";
		}
	}

	nlohmann::json j = {
		{"status", response.status},
		{"rooms", roomsStream.str()}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	std::stringstream playersStream;

	for (size_t i = 0; i < response.rooms.size(); ++i)
	{
		playersStream << response.rooms[i];
		if (i < response.rooms.size() - 1)
		{
			playersStream << ", ";
		}
	}

	nlohmann::json j = {
		{"PlayersInRoom", playersStream.str()}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
	std::stringstream highScoresStream;

	for (size_t i = 0; i < response.statistics.size(); ++i)
	{
		highScoresStream << response.statistics[i];
		if (i < response.statistics.size() - 1)
		{
			highScoresStream << ", ";
		}
	}

	nlohmann::json j = {
		{"status", response.status},
		{"UserStatistics", ""},  // This will be filled by client
		{"HighScores", highScoresStream.str()}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
	// Combine statistics into a single string
	std::stringstream statsStream;

	for (size_t i = 0; i < response.statistics.size(); ++i)
	{
		statsStream << response.statistics[i];
		if (i < response.statistics.size() - 1)
		{
			statsStream << ", ";
		}
	}

	nlohmann::json j = {
		{"status", response.status},
		{"UserStatistics", statsStream.str()}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
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

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
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

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"correctAnswerId", response.correctAnswerId}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
	nlohmann::json j = {
		{"status", response.status},
		{"question", response.question},
		{"answers", response.answers}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& response)
{
	nlohmann::json j = {
		{"status", response.status}
	};

	std::string jsonString = j.dump();

	return std::vector<unsigned char>(jsonString.begin(), jsonString.end());
}