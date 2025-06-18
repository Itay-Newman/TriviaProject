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

GetQuestionResponse GameManager::getNextQuestion(const std::string& username)
{
	GetQuestionResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);

	try
	{
		// Checking if the user is in an active game
		auto gameIt = m_activeGames.find(username);
		if (gameIt == m_activeGames.end())
		{
			if (!startGame(username, 10))
			{
				return response;
			}
			gameIt = m_activeGames.find(username);
		}

		GameData& gameData = gameIt->second;

		if (gameData.gameFinished || gameData.currentQuestionIndex >= gameData.gameQuestions.size())
		{
			gameData.gameFinished = true;
			return response;
		}

		const Question& currentQuestion = gameData.gameQuestions[gameData.currentQuestionIndex];
		gameData.currentQuestion = currentQuestion;

		// Getting the correct answer index after shuffling the answers
		auto shuffledPair = shuffleAnswers(currentQuestion);
		std::vector<std::string> shuffledAnswers = shuffledPair.first;
		unsigned int correctAnswerIndex = shuffledPair.second;
		this->_correctAnswerId = correctAnswerIndex;

		// getting the time of when the question was sent
		gameData.questionStartTime = std::chrono::system_clock::now();

		response.status = static_cast<unsigned int>(Status::SUCCESS);
		response.question = currentQuestion.questionText;

		response.answers = shuffledAnswers;

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in getNextQuestion: " << e.what() << std::endl;
		response.status = static_cast<unsigned int>(Status::FAILURE);
	}


	return response;
}

SubmitAnswerResponse GameManager::submitAnswer(const std::string& username, unsigned int answerId, unsigned int answerTime)
{
	SubmitAnswerResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);
	response.correctAnswerId = 0;

	try
	{
		// Checking if the user is in an active game
		auto gameIt = m_activeGames.find(username);
		if (gameIt == m_activeGames.end())
		{
			return response;
		}

		GameData& gameData = gameIt->second;

		if (gameData.gameFinished)
		{
			return response;
		}

		const Question& currentQuestion = gameData.currentQuestion;

		bool isCorrect = (answerId == this->_correctAnswerId);

		updateGameStatistics(username, isCorrect, static_cast<double>(answerTime));

		gameData.currentQuestionIndex++;
		gameData.questionsAnswered++;

		// Checking if the game is finished
		if (gameData.currentQuestionIndex >= gameData.gameQuestions.size())
		{
			gameData.gameFinished = true;
			// Saving the final statistics to database
			double avgTime = calculateAverageAnswerTime(username);
			m_database.addGameStatistics(username, avgTime, gameData.correctAnswers, gameData.wrongAnswers);
		}

		response.status = static_cast<unsigned int>(Status::SUCCESS);
		response.correctAnswerId = this->_correctAnswerId;

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in submitAnswer: " << e.what() << std::endl;
		response.status = static_cast<unsigned int>(Status::FAILURE);
	}

	return response;
}

GetGameResultsResponse GameManager::getGameResults(const std::string& username)
{
	GetGameResultsResponse response;
	response.status = static_cast<unsigned int>(Status::FAILURE);

	try
	{
		// Checking if user has/had an active game
		auto gameIt = m_activeGames.find(username);
		if (gameIt == m_activeGames.end())
		{
			return response;
		}

		const GameData& gameData = gameIt->second;

		PlayerResults playerResult;
		playerResult.username = username;
		playerResult.correctAnswerCount = gameData.correctAnswers;
		playerResult.wrongAnswerCount = gameData.wrongAnswers;
		playerResult.averageAnswerTime = static_cast<unsigned int>(calculateAverageAnswerTime(username));

		response.results.push_back(playerResult);
		response.status = static_cast<unsigned int>(Status::SUCCESS);

		if (gameData.gameFinished)
		{
			m_activeGames.erase(gameIt);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in getGameResults: " << e.what() << std::endl;
		response.status = static_cast<unsigned int>(Status::FAILURE);
	}

	return response;
}

bool GameManager::startGame(const std::string& username, unsigned int questionCount)
{
	try
	{
		std::vector<Question> questions = getRandomQuestions(questionCount);
		if (questions.empty())
		{
			std::cerr << "No questions available in database" << std::endl;
			return false;
		}

		initializeGameData(username, questions);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error starting game: " << e.what() << std::endl;
		return false;
	}
}

void GameManager::endGame(const std::string& username)
{
	auto gameIt = m_activeGames.find(username);
	if (gameIt != m_activeGames.end())
	{
		// Saving the final statistics if game was in progress
		if (!gameIt->second.gameFinished && gameIt->second.questionsAnswered > 0)
		{
			double avgTime = calculateAverageAnswerTime(username);
			m_database.addGameStatistics(username, avgTime, gameIt->second.correctAnswers, gameIt->second.wrongAnswers);
		}
		m_activeGames.erase(gameIt);
	}
}

bool GameManager::isUserInGame(const std::string& username) const
{
	return m_activeGames.find(username) != m_activeGames.end();
}

std::vector<Question> GameManager::getRandomQuestions(unsigned int count)
{
	return m_database.getRandomQuestions(count);
}

Question GameManager::getRandomQuestion()
{
	std::vector<Question> questions = m_database.getRandomQuestions(1);
	if (!questions.empty())
	{
		return questions[0];
	}

	// Return empty question if database query failed
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

	m_activeGames[username] = gameData;
}

void GameManager::updateGameStatistics(const std::string& username, bool isCorrect, double answerTime)
{
	auto gameIt = m_activeGames.find(username);
	if (gameIt != m_activeGames.end())
	{
		GameData& gameData = gameIt->second;

		if (isCorrect)
		{
			gameData.correctAnswers++;
		}
		else
		{
			gameData.wrongAnswers++;
		}

		gameData.totalAnswerTime += answerTime;
	}
}

double GameManager::calculateAverageAnswerTime(const std::string& username) const
{
	auto gameIt = m_activeGames.find(username);
	if (gameIt != m_activeGames.end())
	{
		const GameData& gameData = gameIt->second;
		if (gameData.questionsAnswered > 0)
		{
			return gameData.totalAnswerTime / gameData.questionsAnswered;
		}
	}
	return 0.0;
}

std::pair<std::vector<std::string>, unsigned int> GameManager::shuffleAnswers(const Question& question)
{
	std::vector<std::string> answers;
	answers.push_back(question.correctAnswer);

	for (const auto& wrongAnswer : question.wrongAnswers)
	{
		answers.push_back(wrongAnswer);
	}

	std::shuffle(answers.begin(), answers.end(), m_randomGenerator);

	unsigned int correctAnswerIndex = 0;
	for (size_t i = 0; i < answers.size(); ++i)
	{
		if (answers[i] == question.correctAnswer)
		{
			correctAnswerIndex = static_cast<unsigned int>(i);
			break;
		}
	}

	return { answers, correctAnswerIndex };
}