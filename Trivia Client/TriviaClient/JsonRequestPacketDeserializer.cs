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

        public static LoginResponse DeserializeLoginResponse(byte[] buffer)
        {
            return Deserialize<LoginResponse>(buffer);
        }

        public static SignupResponse DeserializeSignupResponse(byte[] buffer)
        {
            return Deserialize<SignupResponse>(buffer);
        }

        public static LogoutResponse DeserializeLogoutResponse(byte[] buffer)
        {
            return Deserialize<LogoutResponse>(buffer);
        }

        public static GetRoomsResponse DeserializeGetRoomsResponse(byte[] buffer)
        {
            return Deserialize<GetRoomsResponse>(buffer);
        }

        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(byte[] buffer)
        {
            return Deserialize<GetPlayersInRoomResponse>(buffer);
        }

        public static JoinRoomResponse DeserializeJoinRoomResponse(byte[] buffer)
        {
            return Deserialize<JoinRoomResponse>(buffer);
        }

        public static CreateRoomResponse DeserializeCreateRoomResponse(byte[] buffer)
        {
            var response = Deserialize<CreateRoomResponse>(buffer);
            if (response.Status < 0)
                throw new InvalidOperationException($"Server returned error status: {response.Status}");
            return response;
        }

        public static GetHighScoreResponse DeserializeGetHighScoreResponse(byte[] buffer)
        {
            return Deserialize<GetHighScoreResponse>(buffer);
        }

        public static GetPersonalStatsResponse DeserializeGetPersonalStatsResponse(byte[] buffer)
        {
            return Deserialize<GetPersonalStatsResponse>(buffer);
        }

        public static CloseRoomResponse DeserializeCloseRoomResponse(byte[] buffer)
        {
            return Deserialize<CloseRoomResponse>(buffer);
        }

        public static StartGameResponse DeserializeStartGameResponse(byte[] buffer)
        {
            return Deserialize<StartGameResponse>(buffer);
        }

        public static GetRoomStateResponse DeserializeGetRoomStateResponse(byte[] buffer)
        {
            return Deserialize<GetRoomStateResponse>(buffer);
        }

        public static LeaveRoomResponse DeserializeLeaveRoomResponse(byte[] buffer)
        {
            return Deserialize<LeaveRoomResponse>(buffer);
        }

        public static LeaveGameResponse DeserializeLeaveGameResponse(byte[] buffer)
        {
            return Deserialize<LeaveGameResponse>(buffer);
        }

        public static GetQuestionResponse DeserializeGetQuestionResponse(byte[] buffer)
        {
            return Deserialize<GetQuestionResponse>(buffer);
        }

        public static SubmitAnswerResponse DeserializeSubmitAnswerResponse(byte[] buffer)
        {
            return Deserialize<SubmitAnswerResponse>(buffer);
        }

        public static GetGameResultsResponse DeserializeGetGameResultsResponse(byte[] buffer)
        {
            return Deserialize<GetGameResultsResponse>(buffer);
        }

        public static ErrorResponse DeserializeErrorResponse(byte[] buffer)
        {
            return Deserialize<ErrorResponse>(buffer);
        }
    }
}