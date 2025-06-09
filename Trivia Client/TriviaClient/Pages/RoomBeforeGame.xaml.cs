using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for RoomBeforeGame.xaml
    /// </summary>
    public partial class RoomBeforeGame : Page
    {
        public RoomBeforeGame(bool isAdmin)
        {
            InitializeComponent();

            if (isAdmin)
            {
                AdminControlsPanel.Visibility = Visibility.Visible;
            }
            else
            {
                AdminControlsPanel.Visibility = Visibility.Collapsed;
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
    }
}
