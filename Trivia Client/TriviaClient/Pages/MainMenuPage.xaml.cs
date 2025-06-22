using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Pages
{
    /// <summary>
    /// Interaction logic for MainMenuPage.xaml
    /// </summary>
    public partial class MainMenuPage : Page
    {
        public MainMenuPage()
        {
            InitializeComponent();
        }

        public void SetControlPanelEnabled(bool enabled)
        {
            ControlPanel.IsEnabled = enabled;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            MainContent.Navigate(new CreateRoom(this));
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            MainContent.Navigate(new JoinRoom(this));
        }

        private void PersonalStats_Click(object sender, RoutedEventArgs e)
        {
            // Logic to view statistics
            MainContent.Navigate(new PersonalStats());
        }

        private void Records_Click(object sender, RoutedEventArgs e)
        {
            MainContent.Navigate(new Records());
        }

        private void Settings_Click(object sender, RoutedEventArgs e)
        {
            MainContent.Navigate(new Settings());
        }

        private void ExitApp_Click(object sender, RoutedEventArgs e)
        {
            // Logic to exit the application
            Application.Current.Shutdown();
        }
    }
}