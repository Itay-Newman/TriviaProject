using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Page
    {
        public JoinRoom()
        {
            InitializeComponent();
        }

        private async void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsList.SelectedItem is RoomData selectedRoom)
            {
                var request = new JoinRoomRequest { RoomId = selectedRoom.Id };
                byte[] requestData = JsonRequestPacketSerializer.SerializeJoinRoomRequest(request);
                byte joinRoomCode = (byte)TriviaClient.RequestCodes.JOIN_ROOM_REQUEST;

                var communicator = ClientCommunicator.Instance;
                if (!await communicator.ConnectAsync())
                {
                    MessageBox.Show("Failed to connect to server.");
                    return;
                }

                await communicator.SendRequestAsync(joinRoomCode, requestData);
                var (responseCode, responseBody) = await communicator.ReadResponseAsync();

                var joinRoomResponse = JsonResponsePacketDeserializer.DeserializeJoinRoomResponse(responseBody);

                if (joinRoomResponse.Status == (uint)TriviaClient.StatusCode.OK)
                {
                    NavigationService.Navigate(new RoomBeforeGame(false));
                }
                else
                {
                    MessageBox.Show("Failed to join room.");
                }
            }
            else
            {
                MessageBox.Show("Please select a room to join.");
            }
        }

        private async void Refresh_Click(object sender, RoutedEventArgs e)
        {
            byte getRoomsCode = (byte)TriviaClient.RequestCodes.GET_ROOMS_REQUEST;

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(getRoomsCode, JsonRequestPacketSerializer.SerializeEmptyRequest());
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var getRoomsResponse = JsonResponsePacketDeserializer.DeserializeGetRoomsResponse(responseBody);

            if (getRoomsResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                // Bind the room list to your UI
                RoomsList.ItemsSource = getRoomsResponse.Rooms;
            }
            else
            {
                MessageBox.Show("Failed to fetch room list.");
            }
        }
    }
}
