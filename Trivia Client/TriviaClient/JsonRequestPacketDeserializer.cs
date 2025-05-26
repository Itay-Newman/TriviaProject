using System;
using System.Text;
using Newtonsoft.Json;

namespace TriviaClient
{
    public class JsonRequestPacketSerializer
    {
        public static byte[] SerializeLoginRequest(LoginRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                username = request.Username,
                password = request.Password
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeSignupRequest(SignupRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                username = request.Username,
                password = request.Password,
                email = request.Email
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeGetPlayersInRoomRequest(GetPlayersInRoomRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomId = request.RoomId
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeJoinRoomRequest(JoinRoomRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomId = request.RoomId
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeCreateRoomRequest(CreateRoomRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomName = request.RoomName,
                maxUsers = request.MaxUsers,
                questionCount = request.QuestionCount,
                answerTimeout = request.AnswerTimeout
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeCloseRoomRequest(CloseRoomRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomId = request.RoomId
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeGetRoomStateRequest(GetRoomStateRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomId = request.RoomId
            });

            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeLeaveRoomRequest(LeaveRoomRequest request)
        {
            var json = JsonConvert.SerializeObject(new
            {
                roomId = request.RoomId
            });

            return Encoding.UTF8.GetBytes(json);
        }

        // For requests that don't need data (just message codes)
        public static byte[] SerializeEmptyRequest()
        {
            return new byte[0];
        }
    }
}