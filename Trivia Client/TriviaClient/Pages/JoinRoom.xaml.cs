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
            // Ensure MainFrame is properly referenced
            var mainWindow = Application.Current.MainWindow as MainWindow;
            if (mainWindow != null)
            {
                mainWindow.MainFrame.Navigate(new RoomBeforeGame());
            }
        }

        private void Refresh_Click(object sender, RoutedEventArgs e)
        {
            // Logic to refresh the room list
            MessageBox.Show("Room list refreshed!");
        }
    }
}
