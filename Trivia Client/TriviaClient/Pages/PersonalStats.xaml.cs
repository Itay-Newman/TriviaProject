using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for PersonalStats.xaml
    /// </summary>
    public partial class PersonalStats : Page
    {
        public PersonalStats()
        {
            InitializeComponent();

            GetHighScores();
        }

        private async void GetHighScores()
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte requestCode = (byte)TriviaClient.RequestCodes.GET_PERSONAL_STATS_REQUEST;

            await communicator.SendRequestAsync(requestCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            if (responseCode != (byte)TriviaClient.ResponseCode.GET_PERSONAL_STATS_RESPONSE)
            {
                MessageBox.Show("Failed to get personal stats.");
                return;
            }

            var personalStatsResponse = JsonResponsePacketDeserializer.DeserializeGetPersonalStatsResponse(responseBody);

            AvgTime.Text = personalStatsResponse.Statistics[1];
            CorrectAnswers.Text = personalStatsResponse.Statistics[2];
            WrongAnswers.Text = personalStatsResponse.Statistics[3];
            GamesPlayed.Text = personalStatsResponse.Statistics[4];
            Score.Text = personalStatsResponse.Statistics[5];
        }
    }
}
