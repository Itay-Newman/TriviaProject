using System.Windows;

namespace TriviaClient.Windows
{
    /// <summary>
    /// Interaction logic for Results.xaml
    /// </summary>
    public partial class Results : Window
    {
        private const double CORRECT_ANSWER_WEIGHT = 10.0;
        private const double WRONG_ANSWER_WEIGHT = -5.0;
        private const double GAMES_WEIGHT = 2.0;
        private const double TIME_WEIGHT_FACTOR = 100.0;
        private Double _averageAnswerTime = 0;

        public class LeaderboardEntry
        {
            public string Username { get; set; }
            public uint CorrectAnswerCount { get; set; }
            public uint WrongAnswerCount { get; set; }
            public double AverageAnswerTime { get; set; }
            public double Score { get; set; }
        }



        public Results(Double avgTime)
        {
            InitializeComponent();

            this._averageAnswerTime = avgTime;

            getResults();
        }

        private void MainMenu_Click(object sender, RoutedEventArgs e)
        {
            var mainWindow = new MainWindow();
            mainWindow.Show();
            mainWindow.NavigateToMainMenuPage();
            this.Close();
        }

        private async void getResults()
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            byte requestCode = (byte)TriviaClient.RequestCodes.GET_GAME_RESULTS_REQUEST;
            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();

            await communicator.SendRequestAsync(requestCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var resultsResponse = JsonResponsePacketDeserializer.DeserializeGetGameResultsResponse(responseBody);

            if (resultsResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                PopulateLeaderboard(resultsResponse);
            }
        }

        private void PopulateLeaderboard(GetGameResultsResponse response)
        {
            int totalGames = 1;
            var entries = new List<LeaderboardEntry>();

            foreach (var stats in response.Results)
            {
                uint correctAnswers = stats.CorrectAnswerCount;
                uint wrongAnswers = stats.WrongAnswerCount;
                double avgTime = stats.AverageAnswerTime;

                uint totalAnswers = correctAnswers + wrongAnswers;
                double correctRatio = totalAnswers > 0 ? (double)correctAnswers / totalAnswers : 0;

                double timeFactor = avgTime > 0 ? (TIME_WEIGHT_FACTOR / avgTime) * correctRatio : 0;

                double correctScore = correctAnswers * CORRECT_ANSWER_WEIGHT;
                double wrongScore = wrongAnswers * WRONG_ANSWER_WEIGHT;
                double gameScore = totalGames * GAMES_WEIGHT;

                double totalScore = correctScore + wrongScore + gameScore + timeFactor;

                entries.Add(new LeaderboardEntry
                {
                    Username = stats.Username,
                    CorrectAnswerCount = correctAnswers,
                    WrongAnswerCount = wrongAnswers,
                    AverageAnswerTime = avgTime,
                    Score = Math.Round(totalScore, 2)
                });
            }

            LeaderboardGrid.ItemsSource = entries.OrderByDescending(e => e.Score).ToList();
        }

    }
}
