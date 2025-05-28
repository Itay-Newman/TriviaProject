#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "Structs.h"

struct PlayerStatistics
{
	std::string username;
	double averageAnswerTime;
	int correctAnswers;
	int wrongAnswers;
	int totalGames;
	double score;
};

class StatisticsManager
{
public:
	explicit StatisticsManager(IDatabase& database);
	~StatisticsManager() = default;

	PlayerStatistics getPlayerStatistics(const std::string& username) const;

	// Get the high score table (top 5 players based on score)
	std::vector<PlayerStatistics> getHighScores() const;

private:
	IDatabase& m_database;

	// Constants for score calculation
	static constexpr double CORRECT_ANSWER_WEIGHT = 10.0;
	static constexpr double WRONG_ANSWER_WEIGHT = -5.0;
	static constexpr double GAMES_WEIGHT = 2.0;
	static constexpr double TIME_WEIGHT_FACTOR = 100.0;
};
