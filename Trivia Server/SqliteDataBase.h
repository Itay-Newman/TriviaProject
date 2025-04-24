#include "IRequestHandler.h"
#include <iostream>
#include "sqlite3.h"

class SqliteDataBase : public IRequestHandler
{
public:
	SqliteDataBase(const std::string& dbPath);
	~SqliteDataBase();
	bool doesUserExist(const std::string& username) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	bool addUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	sqlite3* db;
};