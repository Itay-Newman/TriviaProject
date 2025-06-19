#pragma once

#include "IDatabase.h"
#include "Structs.h"
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <unordered_map>

struct Question
{
	int id;
	std::string questionText;
	std::string correctAnswer;
	std::vector<std::string> wrongAnswers;
};

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswers;
	unsigned int wrongAnswers;
	double totalAnswerTime;
	unsigned int questionsAnswered;
	std::vector<Question> gameQuestions;
	unsigned int currentQuestionIndex;
	bool gameFinished;
	std::chrono::system_clock::time_point questionStartTime;

	// Sync data per question
	std::vector<std::string> currentShuffledAnswers;
	unsigned int correctAnswerIndex = 0;
	unsigned int shuffledAnswersForQuestionIndex = -1;
};

class GameManager
{
public:
	explicit GameManager(IDatabase& database);
	~GameManager() = default;

	GetQuestionResponse getNextQuestion(const std::string& username);
	SubmitAnswerResponse submitAnswer(const std::string& username, unsigned int answerId, unsigned int answerTime);
	GetGameResultsResponse getGameResults(const std::string& username);

	// Game management
	bool startGame(const std::string& username, unsigned int questionCount);
	void endGame(const std::string& username);
	bool isUserInGame(const std::string& username) const;

private:
	std::vector<Question> getRandomQuestions(unsigned int count);
	Question getRandomQuestion();

	void initializeGameData(const std::string& username, const std::vector<Question>& questions);
	void updateGameStatistics(const std::string& username, bool isCorrect, double answerTime);
	double calculateAverageAnswerTime(const std::string& username) const;

	std::pair<std::vector<std::string>, unsigned int> shuffleAnswers(const Question& question);

	IDatabase& m_database;
	std::unordered_map<std::string, GameData> m_activeGames;
	std::unordered_map<std::string, PlayerResults> m_userStats;
	std::mt19937 m_randomGenerator;
};
