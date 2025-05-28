using System.Windows;
using System.Windows.Controls;
using TriviaClient.Pages;

namespace TriviaClient
{
    public partial class SignUpPage : Page
    {
        public SignUpPage()
        {
            InitializeComponent();
        }

        private async void SignUpButton_Click(object sender, RoutedEventArgs e)
        {
            string username = Username.Text;
            string password = Password.Text;
            string email = Email.Text;

            var request = new SignupRequest
            {
                Username = username,
                Password = password,
                Email = email
            };

            byte[] requestData = JsonRequestPacketSerializer.SerializeSignupRequest(request);
            byte signupCode = (byte)TriviaClient.RequestCodes.SIGNUP_REQUEST;

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(signupCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var signupResponse = JsonResponsePacketDeserializer.DeserializeSignupResponse(responseBody);

            if (signupResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                MessageBox.Show("Sign up successful! You can now sign in.");
                NavigationService.Navigate(new MainMenuPage());
            }
            else
            {
                MessageBox.Show("Sign up failed. Please check your details or try a different username.");
            }
        }
    }
}
