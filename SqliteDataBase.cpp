#include "SqliteDataBase.h"
#include <iostream>

int countCallback(void* data, int argc, char** argv, char** azColName)
{
	int* count = static_cast<int*>(data);
	if (argv[0] && argc == 1)
	{
		*count = std::stoi(argv[0]);
	}

	return 0;
}

SqliteDataBase::SqliteDataBase(const std::string& dbPath)
{
	// Open the database
	int rc = sqlite3_open(dbPath.c_str(), &this->db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->db));
		sqlite3_close(db);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
}

SqliteDataBase::~SqliteDataBase()
{
	// Close the database
	sqlite3_close(this->db);
}

bool SqliteDataBase::doesUserExist(const std::string& username) const
{
	int count = 0;
	char** err;

	std::string query = "SELECT COUNT(*) FROM users WHERE username = " + username + ";";
	sqlite3_exec(this->db, query.c_str(), &countCallback, &count, err);

	if (count == 1)
	{
		return true;
	}

	return false;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return false;
}

bool SqliteDataBase::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return false;
}
