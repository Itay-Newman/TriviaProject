#include "SqliteDataBase.h"
#include <iostream>
#include "sqlite3.h"
#include <io.h>

int countCallback(void* data, int argc, char** argv, char** azColName)
{
	int* count = static_cast<int*>(data);
	if (argv[0] && argc == 1)
	{
		*count = std::stoi(argv[0]);
	}
	return 0;
}

int passwordCallback(void* data, int argc, char** argv, char** azColName)
{
	std::string* password = static_cast<std::string*>(data);
	if (argv[0] && argc == 1)
	{
		*password = argv[0];
	}
	return 0;
}

int doubleCallback(void* data, int argc, char** argv, char** azColName)
{
	double* value = static_cast<double*>(data);
	if (argv[0] && argc == 1)
	{
		*value = std::stod(argv[0]);
	}
	return 0;
}

int intCallback(void* data, int argc, char** argv, char** azColName)
{
	int* value = static_cast<int*>(data);
	if (argv[0] && argc == 1)
	{
		*value = std::stoi(argv[0]);
	}
	return 0;
}

int stringVectorCallback(void* data, int argc, char** argv, char** azColName)
{
	std::vector<std::string>* strings = static_cast<std::vector<std::string>*>(data);
	if (argv[0] && argc == 1)
	{
		strings->push_back(argv[0]);
	}
	return 0;
}


SqliteDataBase::SqliteDataBase(const std::string& dbPath)
{
	char* errMessage;
	int doesFileExist = _access(dbPath.c_str(), 0);
	int res = sqlite3_open(dbPath.c_str(), &db);

	if (res != SQLITE_OK)
	{
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
	}

	if (doesFileExist != 0)
	{
		std::string sqlStatement = "CREATE TABLE IF NOT EXISTS Users (UserName TEXT NOT NULL UNIQUE, Password TEXT NOT NULL, Email TEXT NOT NULL, PRIMARY KEY(UserName));";
		res = sqlite3_exec(db, sqlStatement.c_str(), NULL, NULL, &errMessage);
		if (res != SQLITE_OK)
		{
			std::cout << "Failed to open DB " << errMessage << errMessage << std::endl;
			if (std::remove(dbPath.c_str()) != 0)
			{
				std::cout << "Failed to delete the database";
			}
		}
	}
}

SqliteDataBase::~SqliteDataBase()
{
	sqlite3_close(this->db);
}

bool SqliteDataBase::doesUserExist(const std::string& username) const
{
	int count = 0;
	char* err = nullptr;

	std::string query = "SELECT COUNT(*) FROM users WHERE username = '" + username + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), countCallback, &count, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return count == 1;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	std::string passwordMatch;
	char* err = nullptr;

	std::string query = "SELECT password FROM users WHERE username = '" + username
		+ "' AND password = '" + password + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), passwordCallback, &passwordMatch, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return !passwordMatch.empty();
}

bool SqliteDataBase::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	std::string query = "INSERT INTO users (username, password, email) VALUES ('"
		+ username + "', '" + password + "', '" + email + "');";

	char* err = nullptr;
	int rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return true;
}

bool SqliteDataBase::initializeUserStatistics(const std::string& username) const
{
	std::string query = "INSERT INTO Statistics (AVG_TIME, C_ANSWERS, W_ANSWER, GAMES, USERNAME) VALUES (0, 0, 0, 0, '" + username + "');";

	char* err = nullptr;
	int rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return true;
}

bool SqliteDataBase::addQuestions(std::string q, std::string a1, std::string a2, std::string a3, std::string a4)
{
	std::string sqlStatementQuestions = "INSERT INTO Questions (QUESTION, C_ANSWER1, W_ANSWER2, W_ANSWER3, W_ANSWER4) VALUES ('" + q + "', '" + a1 + "', '" + a2 + "', '" + a3 + "', '" + a4 + "');";

	char* err = nullptr;
	int rc = sqlite3_exec(this->db, sqlStatementQuestions.c_str(), nullptr, nullptr, &err);
	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL Error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return true;
}

double SqliteDataBase::getPlayerAverageAnswerTime(const std::string& username) const
{
	double avgTime = 0.0;
	char* err = nullptr;

	std::string query = "SELECT AVG_TIME FROM Statistics WHERE USERNAME = '" + username + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), doubleCallback, &avgTime, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return 0.0;
	}

	return avgTime;
}

int SqliteDataBase::getNumOfCorrectAnswers(const std::string& username) const
{
	int correctAnswers = 0;
	char* err = nullptr;

	std::string query = "SELECT C_ANSWERS FROM Statistics WHERE USERNAME = '" + username + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), intCallback, &correctAnswers, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return 0;
	}

	return correctAnswers;
}

int SqliteDataBase::getNumOfWrongAnswers(const std::string& username) const
{
	int wrongAnswers = 0;
	char* err = nullptr;

	std::string query = "SELECT W_ANSWER FROM Statistics WHERE USERNAME = '" + username + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), intCallback, &wrongAnswers, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return 0;
	}

	return wrongAnswers;
}

int SqliteDataBase::getNumOfPlayerGames(const std::string& username) const
{
	int games = 0;
	char* err = nullptr;

	std::string query = "SELECT GAMES FROM Statistics WHERE USERNAME = '" + username + "';";
	int rc = sqlite3_exec(this->db, query.c_str(), intCallback, &games, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return 0;
	}

	return games;
}

bool SqliteDataBase::addGameStatistics(const std::string& username, double avgTime, int correctAnswers, int wrongAnswers)
{
	// Getting current statistics
	double currentAvgTime = getPlayerAverageAnswerTime(username);
	int currentCorrectAnswers = getNumOfCorrectAnswers(username);
	int currentWrongAnswers = getNumOfWrongAnswers(username);
	int currentGames = getNumOfPlayerGames(username);

	// Calculate new statistics
	int newGames = currentGames + 1;
	double newAvgTime;

	if (currentGames == 0) {
		newAvgTime = avgTime;
	}
	else {
		newAvgTime = ((currentAvgTime * currentGames) + avgTime) / newGames;
	}

	int newCorrectAnswers = currentCorrectAnswers + correctAnswers;
	int newWrongAnswers = currentWrongAnswers + wrongAnswers;

	// Update statistics
	std::string query = "UPDATE Statistics SET AVG_TIME = " + std::to_string(newAvgTime) +
		", C_ANSWERS = " + std::to_string(newCorrectAnswers) +
		", W_ANSWER = " + std::to_string(newWrongAnswers) +
		", GAMES = " + std::to_string(newGames) +
		" WHERE USERNAME = '" + username + "';";

	char* err = nullptr;
	int rc = sqlite3_exec(this->db, query.c_str(), nullptr, nullptr, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return true;
}

std::vector<std::string> SqliteDataBase::getPlayersWithStatistics() const
{
	std::vector<std::string> players;
	char* err = nullptr;

	std::string query = "SELECT USERNAME FROM Statistics WHERE GAMES > 0;";
	int rc = sqlite3_exec(this->db, query.c_str(), stringVectorCallback, &players, &err);

	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
	}

	return players;
}
