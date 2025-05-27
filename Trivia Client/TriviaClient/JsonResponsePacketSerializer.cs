using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace TriviaClient
{
    public class JsonResponsePacketDeserializer
    {
        public static LoginResponse DeserializeLoginResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new LoginResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static SignupResponse DeserializeSignupResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new SignupResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static LogoutResponse DeserializeLogoutResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new LogoutResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static GetRoomsResponse DeserializeGetRoomsResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetRoomsResponse
            {
                Status = json["status"].Value<uint>(),
                Rooms = new List<RoomData>()
            };

            if (json["rooms"] != null)
            {
                foreach (var roomJson in json["rooms"])
                {
                    var room = new RoomData
                    {
                        Id = roomJson["id"].Value<uint>(),
                        Name = roomJson["name"].Value<string>(),
                        MaxPlayers = roomJson["maxPlayers"].Value<uint>(),
                        NumOfQuestionsInGame = roomJson["numOfQuestionsInGame"].Value<uint>(),
                        TimePerQuestion = roomJson["timePerQuestion"].Value<uint>(),
                        IsActive = roomJson["isActive"].Value<uint>()
                    };
                    response.Rooms.Add(room);
                }
            }

            return response;
        }

        public static GetPlayersInRoomResponse DeserializeGetPlayersInRoomResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetPlayersInRoomResponse
            {
                Rooms = new List<string>()
            };

            if (json["rooms"] != null)
            {
                if (json["rooms"].Type == JTokenType.Array)
                {
                    foreach (var room in json["rooms"])
                    {
                        response.Rooms.Add(room.Value<string>());
                    }
                }
                else
                {
                    response.Rooms.Add(json["rooms"].Value<string>());
                }
            }

            return response;
        }

        public static JoinRoomResponse DeserializeJoinRoomResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new JoinRoomResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static CreateRoomResponse DeserializeCreateRoomResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new CreateRoomResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static GetHighScoreResponse DeserializeGetHighScoreResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetHighScoreResponse
            {
                Status = json["status"].Value<uint>(),
                Statistics = new List<string>()
            };

            if (json["statistics"] != null)
            {
                if (json["statistics"].Type == JTokenType.Array)
                {
                    foreach (var stat in json["statistics"])
                    {
                        response.Statistics.Add(stat.Value<string>());
                    }
                }
                else
                {
                    response.Statistics.Add(json["statistics"].Value<string>());
                }
            }

            return response;
        }

        public static GetPersonalStatsResponse DeserializeGetPersonalStatsResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetPersonalStatsResponse
            {
                Status = json["status"].Value<uint>(),
                Statistics = new List<string>()
            };

            if (json["statistics"] != null)
            {
                if (json["statistics"].Type == JTokenType.Array)
                {
                    foreach (var stat in json["statistics"])
                    {
                        response.Statistics.Add(stat.Value<string>());
                    }
                }
                else
                {
                    response.Statistics.Add(json["statistics"].Value<string>());
                }
            }

            return response;
        }

        public static CloseRoomResponse DeserializeCloseRoomResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new CloseRoomResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static StartGameResponse DeserializeStartGameResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new StartGameResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static GetRoomStateResponse DeserializeGetRoomStateResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetRoomStateResponse
            {
                Status = json["status"].Value<uint>(),
                HasGameBegun = json["hasGameBegun"].Value<bool>(),
                Players = new List<string>(),
                QuestionCount = json["questionCount"].Value<uint>(),
                AnswerTimeout = json["answerTimeout"].Value<uint>()
            };

            if (json["players"] != null)
            {
                foreach (var player in json["players"])
                {
                    response.Players.Add(player.Value<string>());
                }
            }

            return response;
        }

        public static LeaveRoomResponse DeserializeLeaveRoomResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new LeaveRoomResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static LeaveGameResponse DeserializeLeaveGameResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new LeaveGameResponse
            {
                Status = json["status"].Value<uint>()
            };
        }

        public static GetQuestionResponse DeserializeGetQuestionResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetQuestionResponse
            {
                Status = json["status"].Value<uint>(),
                Question = json["question"].Value<string>(),
                Answers = new List<string>()
            };

            if (json["answers"] != null)
            {
                foreach (var answer in json["answers"])
                {
                    response.Answers.Add(answer.Value<string>());
                }
            }

            return response;
        }

        public static SubmitAnswerResponse DeserializeSubmitAnswerResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new SubmitAnswerResponse
            {
                Status = json["status"].Value<uint>(),
                CorrectAnswerId = json["correctAnswerId"].Value<uint>()
            };
        }

        public static GetGameResultsResponse DeserializeGetGameResultsResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            var response = new GetGameResultsResponse
            {
                Status = json["status"].Value<uint>(),
                Results = new List<PlayerResults>()
            };

            if (json["results"] != null)
            {
                foreach (var resultJson in json["results"])
                {
                    var result = new PlayerResults
                    {
                        Username = resultJson["username"].Value<string>(),
                        CorrectAnswerCount = resultJson["correctAnswerCount"].Value<uint>(),
                        WrongAnswerCount = resultJson["wrongAnswerCount"].Value<uint>(),
                        AverageAnswerTime = resultJson["averageAnswerTime"].Value<uint>()
                    };
                    response.Results.Add(result);
                }
            }

            return response;
        }

        public static ErrorResponse DeserializeErrorResponse(byte[] buffer)
        {
            string jsonStr = Encoding.UTF8.GetString(buffer);
            var json = JObject.Parse(jsonStr);

            return new ErrorResponse
            {
                Status = (ResponseCode)json["status"].Value<int>(),
                Message = json["message"].Value<string>()
            };
        }
    }
}