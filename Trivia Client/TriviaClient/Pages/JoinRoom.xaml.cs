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

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Join Room button clicked! Please enter the room details.");
        }

        private void Refresh_Click(object sender, RoutedEventArgs e)
        {
            // Logic to refresh the room list
            MessageBox.Show("Room list refreshed!");
        }
    }
}
