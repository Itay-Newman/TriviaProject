using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;

namespace TriviaClient.Windows
{
    /// <summary>
    /// Interaction logic for GameWindow.xaml
    /// </summary>
    public partial class GameWindow : Window
    {
        private int _numberOfQuestions = 0;
        private int _timePerQuestion = 0;
        private int _Answer = 0;
        private Stopwatch _questionStopwatch = new Stopwatch();
        private DispatcherTimer _questionTimer;
        private int _correctAnswersCount = 0;
        private int _currentQuestionIndex = 0;
        private Double[] _times = new double[99999999]; //assuming we won't have more than 99999999 questions

        public GameWindow(int numberOfQuestions, int timePerQuestion)
        {
            InitializeComponent();

            this._numberOfQuestions = numberOfQuestions;
            this._timePerQuestion = timePerQuestion;

            CorrectCountText.Text = $"Correct: {this._correctAnswersCount}";
            QuestionCountText.Text = $"Question {this._currentQuestionIndex}/{this._numberOfQuestions}";

            this.ContentRendered += GameWindow_ContentRendered;

            // Initialize the timer
            _questionTimer = new DispatcherTimer();
            _questionTimer.Interval = TimeSpan.FromMilliseconds(200);
            _questionTimer.Tick += QuestionTimer_Tick;
        }

        private async void GameWindow_ContentRendered(object? sender, EventArgs e)
        {
            await GetQuestions();
        }

        private void AnswerButton0_Click(object sender, RoutedEventArgs e)
        {
            this._Answer = 0;
            HighlightSelectedAnswer(AnswerButton1);
        }

        private void AnswerButton1_Click(object sender, RoutedEventArgs e)
        {
            this._Answer = 1;
            HighlightSelectedAnswer(AnswerButton2);
        }

        private void AnswerButton2_Click(object sender, RoutedEventArgs e)
        {
            this._Answer = 2;
            HighlightSelectedAnswer(AnswerButton3);
        }

        private void AnswerButton3_Click(object sender, RoutedEventArgs e)
        {
            this._Answer = 3;
            HighlightSelectedAnswer(AnswerButton4);
        }

        private void HighlightSelectedAnswer(Button selectedButton)
        {
            // Set all buttons to the same (default) color
            var defaultBrush = (Brush)new BrushConverter().ConvertFromString("#8F00FF");
            AnswerButton1.Background = defaultBrush;
            AnswerButton2.Background = defaultBrush;
            AnswerButton3.Background = defaultBrush;
            AnswerButton4.Background = defaultBrush;

            // Set the selected button to a different color
            if (selectedButton != null)
                selectedButton.Background = Brushes.DarkBlue;
        }

        private void ResetHighlight()
        {
            // Reset all buttons to the default color
            var defaultBrush = (Brush)new BrushConverter().ConvertFromString("#8F00FF");
            AnswerButton1.Background = defaultBrush;
            AnswerButton2.Background = defaultBrush;
            AnswerButton3.Background = defaultBrush;
            AnswerButton4.Background = defaultBrush;
        }

        private async void SubmitButton_Click(object sender, RoutedEventArgs e)
        {
            await SubmitAnswerAndProceed();
        }

        private async Task SubmitAnswerAndProceed()
        {
            _questionTimer.Stop();
            _questionStopwatch.Stop();

            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
                return;

            double secondsElapsed = _questionStopwatch.Elapsed.TotalSeconds;
            int msToWait = (int)((this._timePerQuestion - secondsElapsed) * 1000);

            this._times[this._currentQuestionIndex] = secondsElapsed; // Store the time taken for this question

            bool isLastQuestion = (this._currentQuestionIndex == this._numberOfQuestions - 1);

            var request = new SubmitAnswerRequest { answerId = (uint)this._Answer, answerTime = secondsElapsed, isLastQuestion = isLastQuestion };
            byte[] requestData = JsonRequestPacketSerializer.Serialize(request);
            byte requestCode = (byte)TriviaClient.RequestCodes.SUBMIT_ANSWER_REQUEST;

            await communicator.SendRequestAsync(requestCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var response = JsonResponsePacketDeserializer.DeserializeSubmitAnswerResponse(responseBody);

            // Wait for the remaining time, but only if it's positive
            if (msToWait > 0)
                await Task.Delay(msToWait);

            // Disable answer buttons to prevent further input
            SetAnswerButtonsEnabled(false);

            // Color feedback logic
            Button[] buttons = { AnswerButton1, AnswerButton2, AnswerButton3, AnswerButton4 };

            if (response.CorrectAnswerId == this._Answer)
            {
                // Correct: color the selected button green
                buttons[this._Answer].Background = Brushes.Green;
                this._correctAnswersCount++;
            }
            else
            {
                // Incorrect: color the selected button red, correct answer green
                buttons[this._Answer].Background = Brushes.Red;
                if (response.CorrectAnswerId >= 0 && response.CorrectAnswerId < buttons.Length)
                    buttons[response.CorrectAnswerId].Background = Brushes.Green;
            }

            this._currentQuestionIndex++;
            QuestionCountText.Text = $"Question {this._currentQuestionIndex}/{this._numberOfQuestions}";
            CorrectCountText.Text = $"Correct: {_correctAnswersCount}";

            // Wait a moment to show feedback before moving to next question or ending game
            await Task.Delay(1200);

            if (this._currentQuestionIndex == this._numberOfQuestions)
            {
                await Task.Delay(1000);

                double sum = 0;

                for (int i = 0; i < this._numberOfQuestions; i++)
                {
                    sum += this._times[i];
                }

                double averageTime = sum / this._numberOfQuestions;

                var resultsWindow = new Results(averageTime);
                resultsWindow.Show();

                this.Close();
                return;
            }

            await GetQuestions();
        }

        private void SetAnswerButtonsEnabled(bool enabled)
        {
            AnswerButton1.IsEnabled = enabled;
            AnswerButton2.IsEnabled = enabled;
            AnswerButton3.IsEnabled = enabled;
            AnswerButton4.IsEnabled = enabled;
        }

        private async Task GetQuestions()
        {
            var communicator = ClientCommunicator.Instance;

            if (!await communicator.ConnectAsync())
            {
                MessageBox.Show("Failed to connect to server.");
                this.Close();
                return;
            }

            byte[] requestData = JsonRequestPacketSerializer.SerializeEmptyRequest();
            byte getQuestionCode = (byte)TriviaClient.RequestCodes.GET_QUESTION_REQUEST;

            await communicator.SendRequestAsync(getQuestionCode, requestData);
            var (responseCode, responseBody) = await communicator.ReadResponseAsync();

            var questionResponse = JsonResponsePacketDeserializer.DeserializeGetQuestionResponse(responseBody);

            if (questionResponse.Status == (uint)TriviaClient.StatusCode.OK)
            {
                QuestionText.Text = questionResponse.Question;
                QuestionCountText.Text = $"Question {this._currentQuestionIndex}/{_numberOfQuestions}";
                AnswerButton1.Content = questionResponse.Answers[0];
                AnswerButton2.Content = questionResponse.Answers[1];
                AnswerButton3.Content = questionResponse.Answers[2];
                AnswerButton4.Content = questionResponse.Answers[3];

                ResetHighlight();
                SetAnswerButtonsEnabled(true);

                // Start timing for this question
                _questionStopwatch.Restart();
                _questionTimer.Start();
            }
            else
            {
                MessageBox.Show("Failed to retrieve question.");
                this.Close(); // Close the window if we can't get a question
            }
        }

        private async void QuestionTimer_Tick(object? sender, EventArgs e)
        {
            if (_questionStopwatch.Elapsed.TotalSeconds >= _timePerQuestion)
            {
                // Time is up, auto-submit and move to next question
                await SubmitAnswerAndProceed();
            }
            else
            {
                // Update the UI with the remaining time
                double timeLeft = _timePerQuestion - _questionStopwatch.Elapsed.TotalSeconds;
                TimerText.Text = $"Time left: {timeLeft:F2} seconds";
            }
        }
    }
}