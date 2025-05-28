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
            string username = Username.Text;
            string password = Password.Text;

            var request = new LoginRequest { Username = username, Password = password };
            byte[] requestData = JsonRequestPacketSerializer.SerializeLoginRequest(request);

            byte loginCode = (byte)TriviaClient.RequestCodes.LOGIN_REQUEST;

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(loginCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var loginResponse = JsonResponsePacketDeserializer.DeserializeLoginResponse(responseBody);

            if (loginResponse.Status == (uint)TriviaClient.StatusCode.OK)
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