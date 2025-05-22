using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class SignUpPage : Page
    {
        public SignUpPage()
        {
            InitializeComponent();
        }

        private void SignUpButton_Click(object sender, RoutedEventArgs e)
        {
            string username = Username.Text;
            string password = Password.Text;
            string email = Email.Text;

            MessageBox.Show($"Username: {username}\nPassword: {password}\nEmail: {email}", "Sign Up Info");
        }
    }
}
