using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Page
    {
        public CreateRoom()
        {
            InitializeComponent();
        }

        private async void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomName.Text;
            uint maxUsers = uint.Parse(PlayerCount.Text);
            uint questionCount = uint.Parse(QuestionCount.Text);
            uint answerTimeout = uint.Parse(QuestionTime.Text);

            var request = new CreateRoomRequest
            {
                RoomName = roomName,
                MaxUsers = maxUsers,
                QuestionCount = questionCount,
                AnswerTimeout = answerTimeout
            };

            byte[] requestData = JsonRequestPacketSerializer.SerializeCreateRoomRequest(request);
            byte createRoomCode = (byte)TriviaClient.RequestCodes.CREATE_ROOM_REQUEST;

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(createRoomCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var createRoomResponse = JsonResponsePacketDeserializer.DeserializeCreateRoomResponse(responseBody);

            if (createRoomResponse.Status == 1)
            {
                NavigationService.Navigate(new RoomBeforeGame(true));
            }
            else
            {
                MessageBox.Show("Failed to create room.");
            }
        }
    }
}
