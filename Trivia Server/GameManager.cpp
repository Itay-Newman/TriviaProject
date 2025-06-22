#include "GameManager.h"
#include "sqlite3.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

GameManager::GameManager(IDatabase& database)
	: m_database(database), m_randomGenerator(std::chrono::steady_clock::now().time_since_epoch().count())
{
}

bool GameManager::startGame(const std::string& username, unsigned int questionCount)
{
	try {
		if (m_activeGames.find("__shared__") == m_activeGames.end()) {
			std::vector<Question> questions = getRandomQuestions(questionCount);
			if (questions.empty()) {
				std::cerr << "No questions in database\n";
				return false;
			}
			initializeGameData("__shared__", questions);
		}

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error starting game: " << e.what() << std::endl;
		return false;
	}
}

GetQuestionResponse GameManager::getNextQuestion(const std::string& username)
{
	GetQuestionResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);

	try {
		auto gameIt = m_activeGames.find("__shared__");

		if (gameIt == m_activeGames.end()) {
			if (!startGame(username, 10))
				return response;
			gameIt = m_activeGames.find("__shared__");
		}

		GameData& gameData = gameIt->second;

		if (gameData.gameFinished || gameData.currentQuestionIndex >= gameData.gameQuestions.size()) {
			gameData.gameFinished = true;
			return response;
		}

		const Question& currentQuestion = gameData.gameQuestions[gameData.currentQuestionIndex];
		gameData.currentQuestion = currentQuestion;
		gameData.questionStartTime = std::chrono::system_clock::now();

		// Shuffle answers once per question index
		if (gameData.shuffledAnswersForQuestionIndex != gameData.currentQuestionIndex) {
			auto shuffledPair = shuffleAnswers(currentQuestion);
			gameData.currentShuffledAnswers = shuffledPair.first;
			gameData.correctAnswerIndex = shuffledPair.second;
			gameData.shuffledAnswersForQuestionIndex = gameData.currentQuestionIndex;
		}

		response.answers = gameData.currentShuffledAnswers;
		response.question = currentQuestion.questionText;
		response.status = static_cast<unsigned int>(Status::SUCCESS);
	}
	catch (const std::exception& e) {
		std::cerr << "Error in getNextQuestion: " << e.what() << std::endl;
	}

	return response;
}

SubmitAnswerResponse GameManager::submitAnswer(const std::string& username, unsigned int answerId, double answerTime, bool isLast)
{
	SubmitAnswerResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);

	try {
		auto gameIt = m_activeGames.find("__shared__");
		if (gameIt == m_activeGames.end()) return response;

		GameData& gameData = gameIt->second;
		if (gameData.gameFinished) return response;

		if (m_userStats.find(username) == m_userStats.end())
		{
			m_userStats[username] = PlayerResults{ username, 0, 0, 0 };
		}

		unsigned int correctId = gameData.correctAnswerIndex;
		bool isCorrect = (answerId == correctId);

		auto& stats = m_userStats[username];
		if (isCorrect) stats.correctAnswerCount++;
		else stats.wrongAnswerCount++;

		stats.averageAnswerTime += answerTime;

		gameData.currentQuestionIndex++;

		if (gameData.currentQuestionIndex >= gameData.gameQuestions.size() || isLast) {
			gameData.gameFinished = true;
		}

		response.status = static_cast<unsigned int>(Status::SUCCESS);
		response.correctAnswerId = correctId;
	}
	catch (const std::exception& e) {
		std::cerr << "Error in submitAnswer: " << e.what() << std::endl;
	}

	return response;
}

GetGameResultsResponse GameManager::getGameResults(const std::string& username)
{
	this->statsSaved = false;

	GetGameResultsResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);

	try {
		if (m_userStats.empty()) return response;

		bool gameFinished = m_activeGames.contains("__shared__") && m_activeGames["__shared__"].gameFinished;

		for (auto& [user, stats] : m_userStats)
		{
			unsigned int totalQuestions = stats.correctAnswerCount + stats.wrongAnswerCount;
			double avgTime = stats.averageAnswerTime / totalQuestions;

			PlayerResults result = stats;
			result.averageAnswerTime = avgTime;
			response.results.push_back(result);
		}

		if (gameFinished && statsSaved)
		{
			for (auto& [user, stats] : m_userStats)
			{
				unsigned int totalQuestions = stats.correctAnswerCount + stats.wrongAnswerCount;
				double avgTime = stats.averageAnswerTime / totalQuestions;

				m_database.addGameStatistics(user, avgTime, stats.correctAnswerCount, stats.wrongAnswerCount);
			}
			statsSaved = true;
		}

		response.status = static_cast<unsigned int>(Status::SUCCESS);
	}
	catch (const std::exception& e) {
		std::cerr << "Error in getGameResults: " << e.what() << std::endl;
	}

	return response;
}



void GameManager::endGame(const std::string& username)
{
	m_userStats.erase(username);
}

bool GameManager::isUserInGame(const std::string& username) const
{
	return m_userStats.contains(username);
}

std::vector<Question> GameManager::getRandomQuestions(unsigned int count)
{
	return m_database.getRandomQuestions(count);
}

Question GameManager::getRandomQuestion()
{
	std::vector<Question> questions = m_database.getRandomQuestions(1);
	if (!questions.empty()) return questions[0];

	Question emptyQuestion;
	emptyQuestion.id = 0;
	emptyQuestion.questionText = "";
	emptyQuestion.correctAnswer = "";
	emptyQuestion.wrongAnswers.clear();
	return emptyQuestion;
}

void GameManager::initializeGameData(const std::string& username, const std::vector<Question>& questions)
{
	GameData gameData;
	gameData.correctAnswers = 0;
	gameData.wrongAnswers = 0;
	gameData.totalAnswerTime = 0.0;
	gameData.questionsAnswered = 0;
	gameData.gameQuestions = questions;
	gameData.currentQuestionIndex = 0;
	gameData.gameFinished = false;
	gameData.questionStartTime = std::chrono::system_clock::now();
	gameData.currentShuffledAnswers.clear();
	gameData.correctAnswerIndex = 0;
	gameData.shuffledAnswersForQuestionIndex = -1;

	m_activeGames[username] = gameData;
}

std::pair<std::vector<std::string>, unsigned int> GameManager::shuffleAnswers(const Question& question)
{
	std::vector<std::string> answers;
	answers.push_back(question.correctAnswer);
	for (const auto& wrong : question.wrongAnswers) {
		answers.push_back(wrong);
	}
	std::shuffle(answers.begin(), answers.end(), m_randomGenerator);

	unsigned int correctIndex = 0;
	for (size_t i = 0; i < answers.size(); ++i) {
		if (answers[i] == question.correctAnswer) {
			correctIndex = static_cast<unsigned int>(i);
			break;
		}
	}

	return { answers, correctIndex };
}
