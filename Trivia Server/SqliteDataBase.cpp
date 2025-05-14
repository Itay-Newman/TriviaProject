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
