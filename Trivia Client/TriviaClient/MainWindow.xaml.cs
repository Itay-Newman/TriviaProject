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
            MainFrame.Navigate(new MainMenuPage());
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
    }
}
