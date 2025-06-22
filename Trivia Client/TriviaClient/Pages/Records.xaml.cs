using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for Records.xaml
    /// </summary>
    public partial class Records : Page
    {
        public class HighScoreEntry
        {
            public string Username { get; set; }
            public uint Score { get; set; }
        }

        public Records()
        {
            InitializeComponent();

            GetHighScores();
        }

        private async void GetHighScores()
        {
            var entries = new List<HighScoreEntry>();

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            var request = new GetHighScoresRequest { };
            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte requestCode = (byte)TriviaClient.RequestCodes.GET_HIGH_SCORES_REQUEST;

            await communicator.SendRequestAsync(requestCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var response = JsonResponsePacketDeserializer.DeserializeGetHighScoreResponse(responseBody);

            if (response.Status == (uint)TriviaClient.StatusCode.OK)
            {
                for (int i = 1; i < response.Statistics.Count; i++) // start at 1 to skip "High Scores:"
                {
                    string entry = response.Statistics[i];
                    // Example format: "1. itay1 - 21.946519"

                    try
                    {
                        int dotIndex = entry.IndexOf('.');
                        int dashIndex = entry.LastIndexOf('-');

                        string username = entry.Substring(dotIndex + 2, dashIndex - dotIndex - 3).Trim();
                        string scoreStr = entry.Substring(dashIndex + 1).Trim();

                        if (uint.TryParse(scoreStr.Split('.')[0], out uint score))
                        {
                            entries.Add(new HighScoreEntry
                            {
                                Username = username,
                                Score = score
                            });
                        }
                    }
                    catch
                    {
                        // Skip malformed entries
                    }
                }

                LeaderBoardGrid.ItemsSource = entries;
            }
            else
            {
                MessageBox.Show("Failed to fetch high scores.");
            }
        }

    }
}
