#pragma once

#include "IRequestHandler.h"
#include <iostream>
#include "sqlite3.h"
#include "IDatabase.h"

#define DEFAULT_QUESTION_NUMBER 10

class SqliteDataBase : public IDatabase
{
public:
	explicit SqliteDataBase(const std::string& dbPath);
	~SqliteDataBase();
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;
	bool addQuestions(std::string  q, std::string a1, std::string a2, std::string a3, std::string a4);
private:
	sqlite3* db;
};