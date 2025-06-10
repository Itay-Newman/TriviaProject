using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for RoomBeforeGame.xaml
    /// </summary>
    public partial class RoomBeforeGame : Page
    {
        private Thread _refreshThread;
        private bool _keepRefreshing = true;
        private uint _roomId;

        public RoomBeforeGame(bool isAdmin)
        {
            InitializeComponent();
            StartRefreshThread();

            this.Unloaded += RoomBeforeGame_Unloaded;

            if (isAdmin)
            {
                AdminControlsPanel.Visibility = Visibility.Visible;
                LeaveRoom.Visibility = Visibility.Collapsed;
            }
            else
            {
                AdminControlsPanel.Visibility = Visibility.Collapsed;
                LeaveRoom.Visibility = Visibility.Visible;
            }
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Leaving room is not implemented yet.");
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Starting game is not implemented yet.");
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Closing room is not implemented yet.");
        }

        private void StartRefreshThread()
        {
            _refreshThread = new Thread(async () =>
            {
                while (_keepRefreshing)
                {
                    await Application.Current.Dispatcher.InvokeAsync(async () =>
                    {
                        var communicator = ClientCommunicator.Instance;

                        if (!await communicator.ConnectAsync())
                            return;

                        var request = new GetPlayersInRoomRequest { };
                        byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
                        byte getPlayersCode = (byte)TriviaClient.RequestCodes.GET_PLAYERS_IN_ROOM_REQUEST;

                        await communicator.SendRequestAsync(getPlayersCode, requestData);
                        var (responseCode, responseBody) = await communicator.ReadResponseAsync();

                        var getUsersResponse = JsonResponsePacketDeserializer.DeserializeGetPlayersInRoomResponse(responseBody);

                        if (getUsersResponse.Status == (uint)TriviaClient.StatusCode.OK)
                        {
                            PlayerList.ItemsSource = getUsersResponse.users;
                        }
                        else
                        {
                            MessageBox.Show("Failed to retrieve players in the room.");
                        }
                    });

                    Thread.Sleep(3000);
                }
            });

            _refreshThread.IsBackground = true;
            _refreshThread.Start();
        }

        private void RoomBeforeGame_Unloaded(object sender, RoutedEventArgs e)
        {
            _keepRefreshing = false;
            _refreshThread?.Join();
        }
    }
}
