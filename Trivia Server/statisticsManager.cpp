#include "StatisticsManager.h"
#include <algorithm>

StatisticsManager::StatisticsManager(IDatabase& database) : m_database(database)
{
}

PlayerStatistics StatisticsManager::getPlayerStatistics(const std::string& username) const
{
    PlayerStatistics stats;
    stats.username = username;

    // Get statistics from database
    stats.averageAnswerTime = dynamic_cast<SqliteDataBase&>(m_database).getPlayerAverageAnswerTime(username);
    stats.correctAnswers = dynamic_cast<SqliteDataBase&>(m_database).getNumOfCorrectAnswers(username);
    stats.wrongAnswers = dynamic_cast<SqliteDataBase&>(m_database).getNumOfWrongAnswers(username);
    stats.totalGames = dynamic_cast<SqliteDataBase&>(m_database).getNumOfPlayerGames(username);

    // We'll reward faster average answer times
    if (stats.totalGames > 0 && stats.averageAnswerTime > 0)
    {
        double correctAnswerRatio = (stats.correctAnswers + stats.wrongAnswers > 0) ?
            (double)stats.correctAnswers / (stats.correctAnswers + stats.wrongAnswers) : 0;

        // Score formula: (correctAnswers * 10) - (wrongAnswers * 5) + (games * 2) + (100 / avgTime) * correctAnswerRatio
        stats.score = (stats.correctAnswers * 10) - (stats.wrongAnswers * 5) +
            (stats.totalGames * 2) + (100 / stats.averageAnswerTime) * correctAnswerRatio;
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

    // Get all players with statistics
    std::vector<std::string> players = dynamic_cast<SqliteDataBase&>(m_database).getPlayersWithStatistics();

    // Get statistics for each player
    for (const auto& player : players)
    {
        highScores.push_back(getPlayerStatistics(player));
    }

    std::sort(highScores.begin(), highScores.end(),
        [](const PlayerStatistics& a, const PlayerStatistics& b)
        {
            return a.score > b.score;
        });

    if (highScores.size() > 5) {
        highScores.resize(5);
    }

    return highScores;
}