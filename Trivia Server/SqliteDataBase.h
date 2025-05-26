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
	double getPlayerAverageAnswerTime(const std::string& username) const;
	int getNumOfCorrectAnswers(const std::string& username) const;
	int getNumOfWrongAnswers(const std::string& username) const;
	int getNumOfPlayerGames(const std::string& username) const;

	bool addQuestions(std::string q, std::string a1, std::string a2, std::string a3, std::string a4);

	bool addGameStatistics(const std::string& username, double avgTime, int correctAnswers, int wrongAnswers);

	bool initializeUserStatistics(const std::string& username) const;

	std::vector<std::string> getPlayersWithStatistics() const;

private:
	sqlite3* db;
};