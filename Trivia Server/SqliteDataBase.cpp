#include "SqliteDataBase.h"
#include "GameManager.h"
#include <iostream>
#include "sqlite3.h"
#include <io.h>

// Callbacks for sqlite3_exec

int countCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0])
	{
		int* count = static_cast<int*>(data);
		*count = std::stoi(argv[0]);
	}
	return 0;
}

int stringCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0])
	{
		std::string* result = static_cast<std::string*>(data);
		*result = argv[0];
	}
	return 0;
}

int doubleCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0])
	{
		double* value = static_cast<double*>(data);
		*value = std::stod(argv[0]);
	}
	return 0;
}

int intCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0])
	{
		int* value = static_cast<int*>(data);
		*value = std::stoi(argv[0]);
	}
	return 0;
}

int stringVectorCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0])
	{
		auto* vec = static_cast<std::vector<std::string>*>(data);
		vec->push_back(argv[0]);
	}
	return 0;
}

int questionCallback(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 5 && argv[0] && argv[1] && argv[2] && argv[3] && argv[4])
	{
		auto* questions = static_cast<std::vector<Question>*>(data);
		Question question;

		question.id = std::stoi(argv[0]);
		question.questionText = argv[1];
		question.correctAnswer = argv[2];

		question.wrongAnswers.clear();
		question.wrongAnswers.push_back(argv[3]);
		question.wrongAnswers.push_back(argv[4]);

		questions->push_back(question);
	}
	return 0;
}

static bool runQuery(sqlite3* db, const std::string& query, int (*callback)(void*, int, char**, char**), void* data)
{
	char* err = nullptr;
	int rc = sqlite3_exec(db, query.c_str(), callback, data, &err);
	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}
	return true;
}

static bool runPreparedStatement(sqlite3* db, const std::string& sql, const std::vector<std::string>& params)
{
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		return false;
	}

	for (size_t i = 0; i < params.size(); ++i)
	{
		if (sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			std::cerr << "Failed to bind param " << i + 1 << ": " << sqlite3_errmsg(db) << std::endl;
			sqlite3_finalize(stmt);
			return false;
		}
	}

	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE && rc != SQLITE_ROW)
	{
		std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

SqliteDataBase::SqliteDataBase(const std::string& dbPath)
{
	int doesFileExist = _access(dbPath.c_str(), 0);
	int res = sqlite3_open(dbPath.c_str(), &this->db);

	if (res != SQLITE_OK)
	{
		this->db = nullptr;
		std::cerr << "Failed to open DB: " << sqlite3_errmsg(this->db) << std::endl;
		return;
	}

	if (doesFileExist != 0)
	{
		const char* createUsersTable =
			"CREATE TABLE IF NOT EXISTS Users (UserName TEXT NOT NULL UNIQUE, Password TEXT NOT NULL, Email TEXT NOT NULL, PRIMARY KEY(UserName));";
		if (!runQuery(this->db, createUsersTable, nullptr, nullptr))
		{
			std::cerr << "Failed to create Users table" << std::endl;
			sqlite3_close(this->db);
			this->db = nullptr;
			std::remove(dbPath.c_str());
			return;
		}

		const char* createStatisticsTable =
			"CREATE TABLE IF NOT EXISTS Statistics (USERNAME TEXT PRIMARY KEY, AVG_TIME REAL DEFAULT 0, C_ANSWERS INTEGER DEFAULT 0, W_ANSWER INTEGER DEFAULT 0, GAMES INTEGER DEFAULT 0, "
			"FOREIGN KEY(USERNAME) REFERENCES Users(UserName));";
		if (!runQuery(this->db, createStatisticsTable, nullptr, nullptr))
		{
			std::cerr << "Failed to create Statistics table" << std::endl;
			sqlite3_close(this->db);
			this->db = nullptr;
			std::remove(dbPath.c_str());
			return;
		}

		const char* createQuestionsTable =
			"CREATE TABLE IF NOT EXISTS Questions (ID INTEGER PRIMARY KEY AUTOINCREMENT, QUESTION TEXT NOT NULL, C_ANSWER1 TEXT NOT NULL, W_ANSWER2 TEXT NOT NULL, W_ANSWER3 TEXT NOT NULL, W_ANSWER4 TEXT NOT NULL);";
		if (!runQuery(this->db, createQuestionsTable, nullptr, nullptr))
		{
			std::cerr << "Failed to create Questions table" << std::endl;
			sqlite3_close(this->db);
			this->db = nullptr;
			std::remove(dbPath.c_str());
			return;
		}

		std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> questions = {
			std::make_tuple("What is the capital of France?", "Paris", "London", "Berlin", "Madrid"),
			std::make_tuple("Which planet is known as the Red Planet?", "Mars", "Venus", "Jupiter", "Saturn"),
			std::make_tuple("What gas do plants absorb from the atmosphere?", "Carbon Dioxide", "Oxygen", "Nitrogen", "Hydrogen"),
			std::make_tuple("Who painted the Mona Lisa?", "Leonardo da Vinci", "Michelangelo", "Raphael", "Donatello"),
			std::make_tuple("What is the largest ocean on Earth?", "Pacific Ocean", "Atlantic Ocean", "Indian Ocean", "Arctic Ocean"),
			std::make_tuple("What is the hardest natural substance?", "Diamond", "Gold", "Iron", "Quartz"),
			std::make_tuple("Which element has the chemical symbol 'O'?", "Oxygen", "Osmium", "Oxide", "Organium"),
			std::make_tuple("In what year did World War II end?", "1945", "1939", "1940", "1942"),
			std::make_tuple("Which country invented paper?", "China", "Egypt", "Greece", "India"),
			std::make_tuple("What is the smallest prime number?", "2", "1", "3", "0"),
			std::make_tuple("Which is the best Trivia project?", "Itay & Eitan", "no", "no", "no")
		};


		for (const auto& question : questions)
		{
			if (!addQuestions(std::get<0>(question), std::get<1>(question), std::get<2>(question), std::get<3>(question), std::get<4>(question)))
			{
				std::cerr << "Failed to insert initial questions" << std::endl;
				sqlite3_close(this->db);
				this->db = nullptr;
				std::remove(dbPath.c_str());
				return;
			}
		}
	}
}

SqliteDataBase::~SqliteDataBase()
{
	if (this->db)
		sqlite3_close(this->db);
}

bool SqliteDataBase::doesUserExist(const std::string& username) const
{
	int count = 0;
	const std::string query = "SELECT COUNT(*) FROM Users WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return false;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		count = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return count == 1;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	std::string storedPassword;
	const std::string query = "SELECT Password FROM Users WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return false;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const unsigned char* text = sqlite3_column_text(stmt, 0);
		if (text)
			storedPassword = reinterpret_cast<const char*>(text);
	}
	sqlite3_finalize(stmt);

	return !storedPassword.empty() && storedPassword == password;
}

bool SqliteDataBase::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	const std::string sql = "INSERT INTO Users (UserName, Password, Email) VALUES (?, ?, ?);";
	bool didWork = runPreparedStatement(this->db, sql, { username, password, email });
	bool didWork2;

	if (didWork)
	{
		didWork2 = initializeUserStatistics(username);
	}

	return didWork && didWork2;
}

bool SqliteDataBase::initializeUserStatistics(const std::string& username) const
{
	const std::string sql = "INSERT INTO Statistics (USERNAME, GAMES, W_ANSWER, C_ANSWERS, AVG_TIME) VALUES (?, ?, ?, ?, ?);";
	std::vector<std::string> params = {
		username,
		std::to_string(0), // GAMES
		std::to_string(0), // W_ANSWER
		std::to_string(0), // C_ANSWERS
		std::to_string(0)  // AVG_TIME
	};
	return runPreparedStatement(this->db, sql, params);
}


bool SqliteDataBase::addQuestions(std::string q, std::string a1, std::string a2, std::string a3, std::string a4)
{
	// Check if the question already exists (by question text and correct answer)
	const std::string checkSql = "SELECT COUNT(*) FROM Questions WHERE QUESTION = ? AND C_ANSWER1 = ?;";
	sqlite3_stmt* stmt = nullptr;
	int count = 0;

	if (sqlite3_prepare_v2(this->db, checkSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, q.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, a1.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		std::cerr << "Failed to prepare statement for checking question existence: " << sqlite3_errmsg(this->db) << std::endl;
		return false;
	}

	if (count > 0)
	{
		// Question already exists
		return false;
	}

	// Insert the new question
	const std::string sql = "INSERT INTO Questions (QUESTION, C_ANSWER1, W_ANSWER2, W_ANSWER3, W_ANSWER4) VALUES (?, ?, ?, ?, ?);";
	return runPreparedStatement(this->db, sql, { q, a1, a2, a3, a4 });
}

double SqliteDataBase::getPlayerAverageAnswerTime(const std::string& username) const
{
	double avgTime = 0.0;
	const std::string query = "SELECT AVG_TIME FROM Statistics WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return 0.0;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		avgTime = sqlite3_column_double(stmt, 0);

	sqlite3_finalize(stmt);
	return avgTime;
}

int SqliteDataBase::getNumOfCorrectAnswers(const std::string& username) const
{
	int correctAnswers = 0;
	const std::string query = "SELECT C_ANSWERS FROM Statistics WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return 0;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		correctAnswers = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return correctAnswers;
}

int SqliteDataBase::getNumOfWrongAnswers(const std::string& username) const
{
	int wrongAnswers = 0;
	const std::string query = "SELECT W_ANSWER FROM Statistics WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return 0;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		wrongAnswers = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return wrongAnswers;
}

int SqliteDataBase::getNumOfPlayerGames(const std::string& username) const
{
	int games = 0;
	const std::string query = "SELECT GAMES FROM Statistics WHERE UserName = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return 0;

	sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		games = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return games;
}

bool SqliteDataBase::addGameStatistics(const std::string& username, double avgTime, int correctAnswers, int wrongAnswers)
{
	double currentAvgTime = this->getPlayerAverageAnswerTime(username);
	int currentCorrectAnswers = this->getNumOfCorrectAnswers(username);
	int currentWrongAnswers = this->getNumOfWrongAnswers(username);
	int currentGames = this->getNumOfPlayerGames(username);

	double newAvgTime = (currentAvgTime * currentGames + avgTime) / (currentGames + 1);
	int newCorrectAnswers = currentCorrectAnswers + correctAnswers;
	int newWrongAnswers = currentWrongAnswers + wrongAnswers;
	int newGames = currentGames + 1;

	const std::string sql =
		"UPDATE Statistics SET AVG_TIME = ?, C_ANSWERS = ?, W_ANSWER = ?, GAMES = ? WHERE UserName = ?;";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(this->db) << std::endl;
		return false;
	}

	sqlite3_bind_double(stmt, 1, newAvgTime);
	sqlite3_bind_int(stmt, 2, newCorrectAnswers);
	sqlite3_bind_int(stmt, 3, newWrongAnswers);
	sqlite3_bind_int(stmt, 4, newGames);
	sqlite3_bind_text(stmt, 5, username.c_str(), -1, SQLITE_TRANSIENT);

	int rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	return rc == SQLITE_DONE;
}

std::vector<std::string> SqliteDataBase::getAllUsernames() const
{
	std::vector<std::string> users;
	const std::string query = "SELECT UserName FROM Users;";
	runQuery(this->db, query, stringVectorCallback, &users);
	return users;
}

std::vector<std::string> SqliteDataBase::getPlayersWithStatistics() const
{
	std::vector<std::string> players;
	const std::string query = "SELECT UserName FROM Statistics;";
	runQuery(db, query, stringVectorCallback, &players);
	return players;
}

std::vector<Question> SqliteDataBase::getRandomQuestions(unsigned int count) const
{
	std::vector<Question> questions;

	if (!this->db)
	{
		std::cerr << "Database is not initialized" << std::endl;
		return questions;
	}

	const std::string query = "SELECT ID, QUESTION, C_ANSWER1, W_ANSWER2, W_ANSWER3, W_ANSWER4 FROM Questions ORDER BY RANDOM() LIMIT ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "Failed to prepare statement for getRandomQuestions: " << sqlite3_errmsg(this->db) << std::endl;
		return questions;
	}

	if (sqlite3_bind_int(stmt, 1, static_cast<int>(count)) != SQLITE_OK)
	{
		std::cerr << "Failed to bind count parameter: " << sqlite3_errmsg(this->db) << std::endl;
		sqlite3_finalize(stmt);
		return questions;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Question question;

		question.id = sqlite3_column_int(stmt, 0);

		const unsigned char* questionText = sqlite3_column_text(stmt, 1);
		if (questionText)
			question.questionText = reinterpret_cast<const char*>(questionText);

		const unsigned char* correctAnswer = sqlite3_column_text(stmt, 2);
		if (correctAnswer)
			question.correctAnswer = reinterpret_cast<const char*>(correctAnswer);

		question.wrongAnswers.clear();

		const unsigned char* wrongAnswer2 = sqlite3_column_text(stmt, 3);
		if (wrongAnswer2)
			question.wrongAnswers.push_back(reinterpret_cast<const char*>(wrongAnswer2));

		const unsigned char* wrongAnswer3 = sqlite3_column_text(stmt, 4);
		if (wrongAnswer3)
			question.wrongAnswers.push_back(reinterpret_cast<const char*>(wrongAnswer3));

		const unsigned char* wrongAnswer4 = sqlite3_column_text(stmt, 5);
		if (wrongAnswer4)
			question.wrongAnswers.push_back(reinterpret_cast<const char*>(wrongAnswer4));

		questions.push_back(question);
	}

	sqlite3_finalize(stmt);

	if (questions.empty())
	{
		std::cerr << "No questions found in database" << std::endl;
	}

	return questions;
}