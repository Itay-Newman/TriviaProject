#pragma once
#include <string>
#include <vector>

struct Question;

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& name) const = 0;
	virtual bool doesPasswordMatch(const std::string& name, const std::string& password) const = 0;
	virtual bool addUser(const std::string& name, const std::string& password, const std::string& email) const = 0;
	virtual std::vector<std::string> getAllUsernames() const = 0;

	virtual double getPlayerAverageAnswerTime(const std::string& username) const = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
	virtual int getNumOfWrongAnswers(const std::string& username) const = 0;
	virtual int getNumOfPlayerGames(const std::string& username) const = 0;

	virtual bool addQuestions(std::string q, std::string a1, std::string a2, std::string a3, std::string a4) = 0;

	virtual bool addGameStatistics(const std::string& username, double avgTime, int correctAnswers, int wrongAnswers) = 0;

	virtual bool initializeUserStatistics(const std::string& username) const = 0;

	virtual std::vector<std::string> getPlayersWithStatistics() const = 0;

	virtual std::vector<Question> getRandomQuestions(unsigned int count) const = 0;
};