#pragma once

#include <string>
#include <vector>
#include <iostream>

// All structs needed for JsonResponsePacketSerializer

//! Structs code
// 0 - ErrorResponse
// 1 - LoginResponse
// 2 - SignupResponse
// 3 - LogoutResponse
// 4 - GetRoomsResponse
// 5 - GetPlayersInRoomResponse
// 6 - GetHighScoreResponse
// 7 - GetPersonalStatsResponse
// 8 - JoinRoomResponse
// 9 - CreateRoomResponse
// 10 - CloseRoomResponse
// 11 - StartGameResponse
// 12 - GetRoomStateResponse
// 13 - LeaveRoomResponse
// 14 - LeaveGameResponse
// 15 - GetQuestionResponse
// 16 - SubmitAnswerResponse
// 17 - PlayerResults
// 18 - GetGameResultsResponse

struct RoomData {
	// To be filled later
};

struct LoginResponse {
	unsigned int status;
};

struct SignupResponse {
	unsigned int status;
};

struct LogoutResponse {
	unsigned int status;
};

struct GetRoomsResponse {
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse {
	std::vector<std::string> rooms;
};

struct GetHighScoreResponse {
	unsigned int status;
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse {
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse {
	unsigned int status;
};

struct CreateRoomResponse {
	unsigned int status;
};

struct CloseRoomResponse {
	unsigned int status;
};

struct StartGameResponse {
	unsigned int status;
};

struct GetRoomStateResponse {
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct LeaveRoomResponse {
	unsigned int status;
};

struct LeaveGameResponse {
	unsigned int status;
};

struct GetQuestionResponse {
	unsigned int status;
	std::string question;
	std::vector<std::string> answers;
};

struct SubmitAnswerResponse {
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResults {
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse {
	unsigned int status;
	std::vector<PlayerResults> results;
};

struct ErrorResponse {
	std::string message;
};
