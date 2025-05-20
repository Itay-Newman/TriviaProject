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
	SIGNUP_REQUEST = 21,
	LOGOUT_REQUEST = 29,
	// Room-related request codes
	GET_ROOMS_REQUEST = 22,
	GET_PLAYERS_IN_ROOM_REQUEST = 23,
	JOIN_ROOM_REQUEST = 24,
	CREATE_ROOM_REQUEST = 25,
	CLOSE_ROOM_REQUEST = 26,
	GET_ROOM_STATE_REQUEST = 27,
	LEAVE_ROOM_REQUEST = 28,
	// More request codes can be added as needed in the future
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

// Room-related request structs
struct GetRoomsRequest
{
	// No additional data needed
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct CloseRoomRequest
{
	unsigned int roomId;
};

struct GetRoomStateRequest
{
	unsigned int roomId;
};

struct LeaveRoomRequest
{
	unsigned int roomId;
};

struct LogoutRequest
{
	// No additional data needed, username is handled by the handler
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