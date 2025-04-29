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
	bool doesUserExist(const std::string& username) const;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const;
	bool addUser(const std::string& username, const std::string& password, const std::string& email) const;

private:
	sqlite3* db;
};