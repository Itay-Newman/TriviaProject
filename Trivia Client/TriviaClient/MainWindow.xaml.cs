using System.Windows;

namespace TriviaClient
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            MainFrame.Navigate(new SignInPage());
        }
    }
}
