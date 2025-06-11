using System.Windows;
using System.Windows.Controls;
using System.Threading;
using System.Threading.Tasks;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for RoomBeforeGame.xaml
    /// </summary>
    public partial class RoomBeforeGame : Page
    {
        private CancellationTokenSource _refreshCancellation;

        public RoomBeforeGame(bool isAdmin)
        {
            InitializeComponent();
            StartRoomRefreshLoop();

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

        private async void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
                return;

            var request = new LeaveRoomRequest { };
            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte leaveRoomCode = (byte)TriviaClient.RequestCodes.LEAVE_ROOM_REQUEST;

            await communicator.SendRequestAsync(leaveRoomCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var leaveRoomResponse = JsonResponsePacketDeserializer.DeserializeLeaveRoomResponse(responseBody);

            if (leaveRoomResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                _refreshCancellation?.Cancel();
                _refreshCancellation?.Dispose();
                NavigationService.Navigate(new JoinRoom());
            }
            else
            {
                MessageBox.Show("Leave Room Failed");
            }
        }

        private async void StartGame_Click(object sender, RoutedEventArgs e)
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
                return;

            var request = new StartGameResponse { };
            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte startGameCode = (byte)TriviaClient.RequestCodes.STARTS_GAME_REQUEST;

            await communicator.SendRequestAsync(startGameCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var startGameResponse = JsonResponsePacketDeserializer.DeserializeStartGameResponse(responseBody);

            if (startGameResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                MessageBox.Show("Game started successfully!");
            }
            else
            {
                MessageBox.Show("Failed to start game.");
            }
        }

        private async void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
                return;

            var request = new CloseRoomRequest { };
            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte closeRoomCode = (byte)TriviaClient.RequestCodes.CLOSE_ROOM_REQUEST;

            await communicator.SendRequestAsync(closeRoomCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var closeRoomResponse = JsonResponsePacketDeserializer.DeserializeCloseRoomResponse(responseBody);

            if (closeRoomResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                _refreshCancellation?.Cancel();
                _refreshCancellation?.Dispose();
                NavigationService.Navigate(new JoinRoom());
            }
            else
            {
                MessageBox.Show("Failed to close room.");
            }
        }

        private void StartRoomRefreshLoop()
        {
            _refreshCancellation = new CancellationTokenSource();
            var token = _refreshCancellation.Token;

            Task.Run(async () =>
            {
                while (!token.IsCancellationRequested)
                {
                    try
                    {
                        var communicator = ClientCommunicator.Instance;

                        if (!await communicator.ConnectAsync())
                            continue;

                        var request = new GetRoomStateRequest { };
                        byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
                        byte getPlayersCode = (byte)TriviaClient.RequestCodes.GET_ROOM_STATE_REQUEST;

                        await communicator.SendRequestAsync(getPlayersCode, requestData);
                        var (responseCode, responseBody) = await communicator.ReadResponseAsync();

                        var getUsersResponse = JsonResponsePacketDeserializer.DeserializeGetRoomStateResponse(responseBody);

                        if (getUsersResponse.Status == (uint)TriviaClient.StatusCode.OK)
                        {
                            await Dispatcher.InvokeAsync(() =>
                            {
                                PlayerList.ItemsSource = getUsersResponse.Players;
                            });
                        }
                        else
                        {
                            await Dispatcher.InvokeAsync(() =>
                            {
                                MessageBox.Show("Failed to retrieve players in the room.");
                            });
                        }
                    }
                    catch
                    {
                        // eat shit and move on
                    }

                    try { await Task.Delay(3000, token); }
                    catch (TaskCanceledException) { break; }
                }
            }, token);
        }

        private void RoomBeforeGame_Unloaded(object sender, RoutedEventArgs e)
        {
            _refreshCancellation?.Cancel();
            _refreshCancellation?.Dispose();
        }
    }
}
