#include "SqliteDataBase.h"
#include <iostream>
#include "sqlite3.h"

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
	int rc = sqlite3_open(dbPath.c_str(), &this->db);
	if (rc)
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(this->db) << std::endl;
		sqlite3_close(this->db);
	}
	else
	{
		std::cout << "Opened database successfully\n";
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
