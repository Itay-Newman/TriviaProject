using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

using Timer = System.Timers.Timer;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Page
    {
        private Timer _timer;
        private bool clicked = false;
        private MainMenuPage m_mainMenu;
        public JoinRoom(MainMenuPage mainMenu)
        {
            InitializeComponent();
            RefreshRooms();
            m_mainMenu = mainMenu;
            this.Unloaded += JoinRoom_Unloaded;
        }

        private async void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            clicked = true;

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
                    // Stop and dispose the timer to stop sending GET_ROOMS_REQUEST
                    _timer?.Stop();
                    _timer?.Dispose();
                    _timer = null;

                    NavigationService.Navigate(new RoomBeforeGame(false, m_mainMenu));

                    m_mainMenu.SetControlPanelEnabled(false);
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

        private void RefreshRooms()
        {
            _timer?.Stop();
            _timer?.Dispose();

            _timer = new Timer(3000);
            _timer.Elapsed += Timer_Elapsed;
            _timer.AutoReset = true;
            _timer.Start();
        }

        private async void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            // Stop timer while refreshing to avoid overlapping calls
            _timer?.Stop();

            // Exit immediately if clicked is true
            if (clicked)
                return;

            try
            {
                var communicator = ClientCommunicator.Instance;
                if (!await communicator.ConnectAsync())
                    return;

                byte getRoomsCode = (byte)TriviaClient.RequestCodes.GET_ROOMS_REQUEST;
                byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();

                await communicator.SendRequestAsync(getRoomsCode, requestData);
                var (responseCode, responseBody) = await communicator.ReadResponseAsync();

                var getRoomsResponse = JsonResponsePacketDeserializer.DeserializeGetRoomsResponse(responseBody);

                if (getRoomsResponse.Status == (uint)TriviaClient.StatusCode.OK)
                {
                    await Dispatcher.InvokeAsync(() =>
                    {
                        RoomsList.ItemsSource = getRoomsResponse.Rooms
                            .Where(r => r.IsActive != 0)
                            .ToList();
                    });
                }
            }
            finally
            {
                // Only restart timer if not clicked
                if (!clicked)
                    _timer?.Start();
            }
        }

        private void JoinRoom_Unloaded(object sender, RoutedEventArgs e)
        {
            clicked = true; // Prevent further async work
            _timer?.Stop();
            _timer?.Dispose();
            _timer = null;
        }
    }
}
