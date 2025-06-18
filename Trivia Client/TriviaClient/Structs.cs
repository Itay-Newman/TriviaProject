namespace TriviaClient
{
    public enum StatusCode
    {
        ERROR = 0,
        OK = 1,
    }

    public enum RoomState
    {
        WAITING_FOR_PLAYERS = 0,
        GAME_IN_PROGRESS = 1,
        CLOSED = 2
    };

    // Enums matching server side
    public enum ResponseCode
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
    }

    public enum RequestCodes
    {
        LOGIN_REQUEST = 20,
        SIGNUP_REQUEST = 21,
        LOGOUT_REQUEST = 29,
        GET_ROOMS_REQUEST = 22,
        GET_PLAYERS_IN_ROOM_REQUEST = 23,
        JOIN_ROOM_REQUEST = 24,
        CREATE_ROOM_REQUEST = 25,
        CLOSE_ROOM_REQUEST = 26,
        GET_ROOM_STATE_REQUEST = 27,
        LEAVE_ROOM_REQUEST = 28,
        GET_STATISTICS_REQUEST = 30,
        STARTS_GAME_REQUEST = 31,
        GET_QUESTION_REQUEST = 32,
        SUBMIT_ANSWER_REQUEST = 34
    }

    // Data structures
    public struct RoomData
    {
        public uint Id { get; set; }
        public string Name { get; set; }
        public uint MaxPlayers { get; set; }
        public uint NumOfQuestionsInGame { get; set; }
        public uint TimePerQuestion { get; set; }
        public uint IsActive { get; set; }
    }

    public struct PlayerResults
    {
        public string Username { get; set; }
        public uint CorrectAnswerCount { get; set; }
        public uint WrongAnswerCount { get; set; }
        public uint AverageAnswerTime { get; set; }
    }

    // Request structs (client sends these)
    public struct LoginRequest
    {
        public string Username { get; set; }
        public string Password { get; set; }
    }

    public struct SignupRequest
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }
    }

    public struct GetPlayersInRoomRequest
    {
        public uint RoomId { get; set; }
    }

    public struct JoinRoomRequest
    {
        public uint RoomId { get; set; }
    }

    public struct CreateRoomRequest
    {
        public string RoomName { get; set; }
        public uint MaxUsers { get; set; }
        public uint QuestionCount { get; set; }
        public uint AnswerTimeout { get; set; }
    }

    public struct CloseRoomRequest
    {
        public uint RoomId { get; set; }
    }

    public struct GetRoomStateRequest
    {
    }

    public struct LeaveRoomRequest
    {
    }

    // Response structs (client receives these)
    public struct LoginResponse
    {
        public uint Status { get; set; }
    }

    public struct SignupResponse
    {
        public uint Status { get; set; }
    }

    public struct LogoutResponse
    {
        public uint Status { get; set; }
    }

    public struct GetRoomsResponse
    {
        public uint Status { get; set; }
        public List<RoomData> Rooms { get; set; }
    }

    public struct GetPlayersInRoomResponse
    {
        public uint Status { get; set; }
        public List<string> users { get; set; }
    }

    public struct GetHighScoreResponse
    {
        public uint Status { get; set; }
        public List<string> Statistics { get; set; }
    }

    public struct GetPersonalStatsResponse
    {
        public uint Status { get; set; }
        public List<string> Statistics { get; set; }
    }

    public struct JoinRoomResponse
    {
        public uint Status { get; set; }
    }

    public struct CreateRoomResponse
    {
        public uint Status { get; set; }
    }

    public struct CloseRoomResponse
    {
        public uint Status { get; set; }
    }

    public struct StartGameResponse
    {
        public uint Status { get; set; }
    }

    public struct GetRoomStateResponse
    {
        public uint Status { get; set; }
        public bool HasGameBegun { get; set; }
        public List<string> Players { get; set; }
        public uint QuestionCount { get; set; }
        public uint AnswerTimeout { get; set; }
    }

    public struct LeaveRoomResponse
    {
        public uint Status { get; set; }
    }

    public struct LeaveGameResponse
    {
        public uint Status { get; set; }
    }

    public struct GetQuestionResponse
    {
        public uint Status { get; set; }
        public string Question { get; set; }
        public List<string> Answers { get; set; }
    }

    public struct SubmitAnswerResponse
    {
        public uint Status { get; set; }
        public uint CorrectAnswerId { get; set; }
    }

    public struct GetGameResultsResponse
    {
        public uint Status { get; set; }
        public List<PlayerResults> Results { get; set; }
    }

    public struct SubmitAnswerRequest
    {
        public uint answerId { get; set; }
        public uint answerTime { get; set; }
    };

    public struct ErrorResponse
    {
        public ResponseCode Status { get; set; }
        public string Message { get; set; }
    }

    public struct RequestInfo
    {
        public uint Id { get; set; }
        public DateTime ReceivalTime { get; set; }
        public byte[] Buffer { get; set; }
    }
}