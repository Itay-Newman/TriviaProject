using System.Text;
using Newtonsoft.Json;
using TriviaClient;

namespace TriviaClient
{
    public static class JsonRequestPacketSerializer
    {
        public static byte[] Serialize<T>(T request)
        {
            string json = JsonConvert.SerializeObject(request);
            return Encoding.UTF8.GetBytes(json);
        }

        public static byte[] SerializeEmptyRequest()
        {
            return new byte[0];
        }

        public static byte[] SerializeSignupRequest(SignupRequest request)
        {
            return Serialize(request);
        }

        public static byte[] SerializeLoginRequest(LoginRequest request)
        {
            return Serialize(request);
        }

        public static byte[] SerializeJoinRoomRequest(JoinRoomRequest request)
        {
            return Serialize(request);
        }

        public static byte[] SerializeCreateRoomRequest(CreateRoomRequest request)
        {
            return Serialize(request);
        }
    }
}