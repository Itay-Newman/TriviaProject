using System.Text;
using Newtonsoft.Json;

namespace TriviaClient
{
    public static class JsonRequestPacketSerializer
    {
        // Generic serializer
        private static byte[] SerializeObject(object obj)
        {
            string json = JsonConvert.SerializeObject(obj);
            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeLoginRequest(LoginRequest request)
        {
            return SerializeObject(new { request.Username, request.Password });
        }

        public static byte[] SerializeSignupRequest(SignupRequest request)
        {
            return SerializeObject(new { request.Username, request.Password, request.Email });
        }

        public static byte[] SerializeGetPlayersInRoomRequest(GetPlayersInRoomRequest request)
        {
            return SerializeObject(new { request.RoomId });
        }

        public static byte[] SerializeJoinRoomRequest(JoinRoomRequest request)
        {
            return SerializeObject(new { request.RoomId });
        }

        public static byte[] SerializeCreateRoomRequest(CreateRoomRequest request)
        {
            return SerializeObject(new
            {
                request.RoomName,
                request.MaxUsers,
                request.QuestionCount,
                request.AnswerTimeout
            });
        }

        public static byte[] SerializeCloseRoomRequest(CloseRoomRequest request)
        {
            return SerializeObject(new { request.RoomId });
        }

        public static byte[] SerializeGetRoomStateRequest(GetRoomStateRequest request)
        {
            return SerializeObject(new { request.RoomId });
        }

        public static byte[] SerializeLeaveRoomRequest(LeaveRoomRequest request)
        {
            return SerializeObject(new { request.RoomId });
        }

        public static byte[] SerializeEmptyRequest()
        {
            return new byte[0];
        }
    }
}
