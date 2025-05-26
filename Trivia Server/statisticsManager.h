#pragma once

#include <string>
#include <vector>
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
};