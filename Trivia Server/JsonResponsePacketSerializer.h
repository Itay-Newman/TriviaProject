#pragma once

#include <vector>
#include "Structs.h"
#include "json.hpp"

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(const ErrorResponse& response);
	static std::vector<unsigned char> serializeResponse(const LoginResponse& response);
	static std::vector<unsigned char> serializeResponse(const SignupResponse& response);

	static std::vector<unsigned char> serializeResponse(const LogoutResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse& response);
	static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& response);
	static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetHighScoreResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetPersonalStatsResponse& response);

	static std::vector<unsigned char> serializeResponse(const CloseRoomResponse& response);
	static std::vector<unsigned char> serializeResponse(const StartGameResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse& response);
	static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse& response);

	static std::vector<unsigned char> serializeResponse(const GetGameResultsResponse& response);
	static std::vector<unsigned char> serializeResponse(const SubmitAnswerResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetQuestionResponse& response);
	static std::vector<unsigned char> serializeResponse(const LeaveGameResponse& response);
};