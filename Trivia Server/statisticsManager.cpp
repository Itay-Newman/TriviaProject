#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase& database) : m_database(database)
{
}

PlayerStatistics StatisticsManager::getPlayerStatistics(const std::string& username) const
{
	PlayerStatistics stats;
	stats.username = username;

	try
	{
		stats.averageAnswerTime = this->m_database.getPlayerAverageAnswerTime(username);
		stats.correctAnswers = this->m_database.getNumOfCorrectAnswers(username);
		stats.wrongAnswers = this->m_database.getNumOfWrongAnswers(username);
		stats.totalGames = this->m_database.getNumOfPlayerGames(username);
	}
	catch (...)
	{
		// In case DB call fails, zero stats
		stats.averageAnswerTime = 0;
		stats.correctAnswers = 0;
		stats.wrongAnswers = 0;
		stats.totalGames = 0;
		stats.score = 0;
		return stats;
	}

	if (stats.totalGames > 0 && stats.averageAnswerTime > 0)
	{
		double totalAnswers = stats.correctAnswers + stats.wrongAnswers;
		double correctAnswerRatio = totalAnswers > 0 ? (double)stats.correctAnswers / totalAnswers : 0;

		stats.score = (stats.correctAnswers * CORRECT_ANSWER_WEIGHT) +
			(stats.wrongAnswers * WRONG_ANSWER_WEIGHT) +
			(stats.totalGames * GAMES_WEIGHT) +
			(TIME_WEIGHT_FACTOR / stats.averageAnswerTime) * correctAnswerRatio;
	}
	else
	{
		stats.score = 0;
	}

	return stats;
}

std::vector<PlayerStatistics> StatisticsManager::getHighScores() const
{
	std::vector<PlayerStatistics> highScores;

	// Safer dynamic_cast check
	auto sqliteDbPtr = dynamic_cast<SqliteDataBase*>(&this->m_database);
	if (!sqliteDbPtr)
	{
		// fallback: return empty or handle error
		return highScores;
	}

	std::vector<std::string> players;
	try
	{
		players = sqliteDbPtr->getPlayersWithStatistics();
	}
	catch (...)
	{
		// DB call failed
		return highScores;
	}

	highScores.reserve(players.size());
	for (const auto& player : players)
	{
		highScores.push_back(getPlayerStatistics(player));
	}

	std::sort(highScores.begin(), highScores.end(),
		[](const PlayerStatistics& a, const PlayerStatistics& b)
		{
			return a.score > b.score;
		});

	if (highScores.size() > 5)
	{
		highScores.resize(5);
	}

	return highScores;
}
