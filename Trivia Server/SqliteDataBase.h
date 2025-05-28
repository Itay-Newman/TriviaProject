#pragma once

#include "IRequestHandler.h"
#include <iostream>
#include "sqlite3.h"
#include "IDatabase.h"

class SqliteDataBase : public IDatabase
{
public:
	explicit SqliteDataBase(const std::string& dbPath);
	~SqliteDataBase();
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

	// Statistics methods
	double getPlayerAverageAnswerTime(const std::string& username) const override;
	int getNumOfCorrectAnswers(const std::string& username) const override;
	int getNumOfWrongAnswers(const std::string& username) const override;
	int getNumOfPlayerGames(const std::string& username) const override;

	bool addQuestions(std::string q, std::string a1, std::string a2, std::string a3, std::string a4) override;

	bool addGameStatistics(const std::string& username, double avgTime, int correctAnswers, int wrongAnswers) override;

	bool initializeUserStatistics(const std::string& username) const override;

	std::vector<std::string> getPlayersWithStatistics() const override;

private:
	sqlite3* db;
};