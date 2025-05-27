using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for SignInPage.xaml
    /// </summary>
    public partial class SignInPage : Page
    {
        public SignInPage()
        {
            InitializeComponent();
        }

        private async void SignInButton_Click(object sender, RoutedEventArgs e)
        {
            // Example: Assume you have TextBoxes named UsernameBox and PasswordBox
            string username = Username.Text;
            string password = Password.Text;

            var request = new LoginRequest { Username = username, Password = password };
            byte[] requestData = JsonRequestPacketSerializer.SerializeLoginRequest(request);

            // Message code for login (replace with your actual code, e.g., 1)
            byte loginCode = 20;

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(loginCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var loginResponse = JsonResponsePacketDeserializer.DeserializeLoginResponse(responseBody);

            if (loginResponse.Status == 1)
            {
                // Success: Navigate to main menu
                NavigationService.Navigate(new Pages.MainMenuPage());
            }
            else
            {
                MessageBox.Show("Login failed. Please check your credentials.");
            }
        }

        private void SignUpButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new SignUpPage());
        }
    }
}