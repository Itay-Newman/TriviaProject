#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

class IRequestHandler;
// All structs needed for JsonResponsePacketSerializer and JsonRequestPacketDeserializer

// There are only 2 statuses: 1 - success, 0 - fail

enum class ResponseCode
{
	ERROR_RESPONSE = 0,
	LOGIN_RESPONSE = 1,
	SIGNUP_RESPONSE = 2,
	LOGOUT_RESPONSE = 3,
	GET_ROOMS_RESPONSE = 4,
	GET_PLAYERS_IN_ROOM_RESPONSE = 5,
	GET_HIGH_SCORE_RESPONSE = 6,
	GET_PERSONAL_STATS_RESPONSE = 7,
	JOIN_ROOM_RESPONSE = 8,
	CREATE_ROOM_RESPONSE = 9,
	CLOSE_ROOM_RESPONSE = 10,
	START_GAME_RESPONSE = 11,
	GET_ROOM_STATE_RESPONSE = 12,
	LEAVE_ROOM_RESPONSE = 13,
	LEAVE_GAME_RESPONSE = 14,
	GET_QUESTION_RESPONSE = 15,
	SUBMIT_ANSWER_RESPONSE = 16,
	PLAYER_RESULTS = 17,
	GET_GAME_RESULTS_RESPONSE = 18
};

// Request Message Codes
enum class RequestCodes
{
	LOGIN_REQUEST = 20,
	SIGNUP_REQUEST = 21
	// Will add more request codes as needed
};

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

// Request structs
struct RequestInfo
{
	unsigned int id;
	std::chrono::system_clock::time_point receivalTime;
	std::vector<unsigned char> buffer;
};

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

// Response structs
struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> rooms;
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};

struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct LeaveRoomResponse
{
	unsigned int status;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::vector<std::string> answers;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
};

struct ErrorResponse
{
	ResponseCode status; // 0 for fail
	std::string message;
};

struct RequestResult
{
	ResponseCode id; // 0 for fail
	std::vector<unsigned char> response;
	IRequestHandler* newHandler;
};
