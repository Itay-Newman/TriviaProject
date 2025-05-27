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
                byte joinRoomCode = 24;

                var communicator = ClientCommunicator.Instance;
                if (!await communicator.ConnectAsync())
                {
                    MessageBox.Show("Failed to connect to server.");
                    return;
                }

                await communicator.SendRequestAsync(joinRoomCode, requestData);
                var (responseCode, responseBody) = await communicator.ReadResponseAsync();

                var joinRoomResponse = JsonResponsePacketDeserializer.DeserializeJoinRoomResponse(responseBody);

                if (joinRoomResponse.Status == 1)
                {
                    MessageBox.Show("Joined room successfully!");
                    // Optionally navigate to the room lobby
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
            byte getRoomsCode = 22; 

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(getRoomsCode, JsonRequestPacketSerializer.SerializeEmptyRequest());
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var getRoomsResponse = JsonResponsePacketDeserializer.DeserializeGetRoomsResponse(responseBody);

            if (getRoomsResponse.Status == 1)
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
