using System;
using System.Threading.Tasks;
using System.Windows;
using TriviaClient.Pages;

namespace TriviaClient
{
    public partial class MainWindow : Window
    {
        public static MainWindow Instance;

        public MainWindow()
        {
            InitializeComponent();
            Instance = this;

            _ = InitializeConnectionAsync();

            MainFrame.Navigate(new MainMenuPage());
        }

        private async Task InitializeConnectionAsync()
        {
            var connected = await ClientCommunicator.Instance.ConnectAsync();
            if (!connected)
            {
                MessageBox.Show("Failed to connect to trivia server. Closing.");
                Application.Current.Shutdown();
            }
            else
            {
                Console.WriteLine("✅ Connected to trivia server.");
            }
        }

        private void BackgroundMusic_MediaEnded(object sender, RoutedEventArgs e)
        {
            BackgroundMusic.Position = TimeSpan.Zero;
            BackgroundMusic.Play();
        }

        public void SetVolume(double volume)
        {
            BackgroundMusic.Volume = volume;
        }

        public double GetVolume()
        {
            return BackgroundMusic.Volume;
        }
    }
}