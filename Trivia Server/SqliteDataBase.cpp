#include "SqliteDataBase.h"
#include <iostream>
#include "sqlite3.h"
#include <io.h>
#include <tuple>


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

SqliteDataBase::SqliteDataBase(const std::string& dbPath)
{
	int doesFileExist = _access(dbPath.c_str(), 0);
	int res = sqlite3_open(dbPath.c_str(), &db);

	if (res != SQLITE_OK)
	{
		db = nullptr;
		throw std::exception("[SQL ERROR] Can't create database file.");
	}

	std::string sqlStatementUsers = "CREATE TABLE IF NOT EXISTS Users (UserName TEXT NOT NULL UNIQUE, Password TEXT NOT NULL, Email TEXT NOT NULL, PRIMARY KEY(UserName));";
	std::string sqlStatementStat = "CREATE TABLE IF NOT EXISTS Statistics (AVG_TIME NUMERIC NOT NULL, C_ANSWERS INTEGER NOT NULL, W_ANSWER INTEGER NOT NULL, GAMES INTEGER NOT NULL, USERNAME TEXT NOT NULL, FOREIGN KEY(USERNAME) REFERENCES Users(UserName));";
	std::string sqlStatementQ = "CREATE TABLE IF NOT EXISTS Questions (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, QUESTION TEXT NOT NULL, C_ANSWER1 TEXT NOT NULL, W_ANSWER2 TEXT NOT NULL, W_ANSWER3 TEXT NOT NULL, W_ANSWER4 TEXT NOT NULL);";


	res = sqlite3_exec(db, sqlStatementUsers.c_str(), NULL, NULL, NULL);
	if (res != SQLITE_OK)
	{
		throw std::exception("[SQL ERROR] Can't create USERS Table.");
	}

	res = sqlite3_exec(db, sqlStatementStat.c_str(), NULL, NULL, NULL);
	if (res != SQLITE_OK)
	{
		throw std::exception("[SQL ERROR] Can't create STATISTICS Table.");
	}

	res = sqlite3_exec(db, sqlStatementQ.c_str(), NULL, NULL, NULL);
	if (res != SQLITE_OK)
	{
		throw std::exception("[SQL ERROR] Can't create QUESTIONS Table.");

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
		std::make_tuple("What is the smallest prime number?", "2", "1", "3", "0")
	};

	for (const auto& question : questions)
	{
		std::string q = std::get<0>(question);
		std::string a1 = std::get<1>(question);
		std::string a2 = std::get<2>(question);
		std::string a3 = std::get<3>(question);
		std::string a4 = std::get<4>(question);
		addQuestions(q, a1, a2, a3, a4);
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

bool SqliteDataBase::addQuestions(std::string  q, std::string a1, std::string a2, std::string a3, std::string a4)
{
	std::string sqlStatementQuestions = "INSERT INTO Questions (question, c_answer1, w_answer2, w_answer3, w_answer4) VALUES ('" + q + "', '" + a1 + "', '" + a2 + "', '" + a3 + "', '" + a4 + "');";

	char* err = nullptr;
	int rc = sqlite3_exec(this->db, sqlStatementQuestions.c_str(), nullptr, nullptr, &err);
	if (rc != SQLITE_OK)
	{
		std::cerr << "SQL Error: " << err << std::endl;
		sqlite3_free(err);
		return false;
	}

	return false;
}
