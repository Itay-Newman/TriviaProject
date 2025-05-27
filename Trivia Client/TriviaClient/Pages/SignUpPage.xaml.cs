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
            byte signupCode = 21; // Adjust if your protocol uses a different code

            var communicator = ClientCommunicator.Instance;
            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                return;
            }

            await communicator.SendRequestAsync(signupCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var signupResponse = JsonResponsePacketDeserializer.DeserializeSignupResponse(responseBody);

            if (signupResponse.Status == 1)
            {
                MessageBox.Show("Sign up successful! You can now sign in.");
                // Optionally navigate to sign-in page
                NavigationService.Navigate(new MainMenuPage()); // Adjust as necessary
            }
            else
            {
                MessageBox.Show("Sign up failed. Please check your details or try a different username.");
            }
        }
    }
}
