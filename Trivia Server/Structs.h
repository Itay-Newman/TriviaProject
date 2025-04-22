#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

// All structs needed for JsonResponsePacketSerializer and JsonRequestPacketDeserializer

// There are only 2 statuses: 1 - success, 0 - fail

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

// Request Message Codes
enum RequestCodes {
    LOGIN_REQUEST = 20,
    SIGNUP_REQUEST = 21
    // Will add more request codes as needed
};

struct RoomData {
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int numOfQuestionsInGame;
    unsigned int timePerQuestion;
    unsigned int isActive;
};

// Request structs
struct RequestInfo {
    int id;
    std::chrono::system_clock::time_point receivalTime;
    std::vector<unsigned char> buffer;
};

struct LoginRequest {
    std::string username;
    std::string password;
};

struct SignupRequest {
    std::string username;
    std::string password;
    std::string email;
};

// Response structs
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
    unsigned int status; // 0 for fail
    std::string message;
};