using System.Text;
using Newtonsoft.Json;

namespace TriviaClient
{
    public static class JsonResponsePacketDeserializer
    {
        private static T Deserialize<T>(byte[] buffer)
        {
            string json = Encoding.UTF8.GetString(buffer);
            return JsonConvert.DeserializeObject<T>(json);
        }

        public static LoginResponse DeserializeLoginResponse(byte[] buffer) =>
            Deserialize<LoginResponse>(buffer);

        public static SignupResponse DeserializeSignupResponse(byte[] buffer) =>
            Deserialize<SignupResponse>(buffer);

        public static LogoutResponse DeserializeLogoutResponse(byte[] buffer) =>
            Deserialize<LogoutResponse>(buffer);

        public static GetRoomsResponse DeserializeGetRoomsResponse(byte[] buffer) =>
            Deserialize<GetRoomsResponse>(buffer);

        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(byte[] buffer) =>
            Deserialize<GetPlayersInRoomResponse>(buffer);

        public static JoinRoomResponse DeserializeJoinRoomResponse(byte[] buffer) =>
            Deserialize<JoinRoomResponse>(buffer);

        public static CreateRoomResponse DeserializeCreateRoomResponse(byte[] buffer)
        {
            var response = Deserialize<CreateRoomResponse>(buffer);
            if (response.Status < 0)
                throw new InvalidOperationException($"Server returned error status: {response.Status}");
            return response;
        }

        public static GetHighScoreResponse DeserializeGetHighScoreResponse(byte[] buffer) =>
            Deserialize<GetHighScoreResponse>(buffer);

        public static GetPersonalStatsResponse DeserializeGetPersonalStatsResponse(byte[] buffer) =>
            Deserialize<GetPersonalStatsResponse>(buffer);

        public static CloseRoomResponse DeserializeCloseRoomResponse(byte[] buffer) =>
            Deserialize<CloseRoomResponse>(buffer);

        public static StartGameResponse DeserializeStartGameResponse(byte[] buffer) =>
            Deserialize<StartGameResponse>(buffer);

        public static GetRoomStateResponse DeserializeGetRoomStateResponse(byte[] buffer) =>
            Deserialize<GetRoomStateResponse>(buffer);

        public static LeaveRoomResponse DeserializeLeaveRoomResponse(byte[] buffer) =>
            Deserialize<LeaveRoomResponse>(buffer);

        public static LeaveGameResponse DeserializeLeaveGameResponse(byte[] buffer) =>
            Deserialize<LeaveGameResponse>(buffer);

        public static GetQuestionResponse DeserializeGetQuestionResponse(byte[] buffer) =>
            Deserialize<GetQuestionResponse>(buffer);

        public static SubmitAnswerResponse DeserializeSubmitAnswerResponse(byte[] buffer) =>
            Deserialize<SubmitAnswerResponse>(buffer);

        public static GetGameResultsResponse DeserializeGetGameResultsResponse(byte[] buffer) =>
            Deserialize<GetGameResultsResponse>(buffer);

        public static ErrorResponse DeserializeErrorResponse(byte[] buffer) =>
            Deserialize<ErrorResponse>(buffer);
    }
}