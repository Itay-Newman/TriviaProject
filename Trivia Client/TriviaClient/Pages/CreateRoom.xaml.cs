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

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            // Ensure MainFrame is properly referenced
            var mainWindow = Application.Current.MainWindow as MainWindow;
            if (mainWindow != null)
            {
                mainWindow.MainFrame.Navigate(new RoomBeforeGame());
            }
        }
    }
}
