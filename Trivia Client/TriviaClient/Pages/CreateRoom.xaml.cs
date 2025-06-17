using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Page
    {

        private MainMenuPage _mainMenu;

        public CreateRoom(MainMenuPage MainMenuParam)
        {
            InitializeComponent();
            TimePerQuestion.Text = "20";
            MaxPlayers.Text = "5";
            QuestionNumbers.Text = "5";

            _mainMenu = MainMenuParam;
        }

        private async void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomName.Text;
            uint maxUsers = uint.Parse(MaxPlayers.Text);
            uint questionCount = uint.Parse(QuestionNumbers.Text);
            uint answerTimeout = uint.Parse(TimePerQuestion.Text);

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
                NavigationService.Navigate(new RoomBeforeGame(true, _mainMenu));

                _mainMenu.SetControlPanelEnabled(false);
            }
            else
            {
                MessageBox.Show("Failed to create room.");
            }
        }

        // Event handler to allow only numeric input
        private void NumericOnly_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !Regex.IsMatch(e.Text, "^[0-9]+$");
        }
    }
}
