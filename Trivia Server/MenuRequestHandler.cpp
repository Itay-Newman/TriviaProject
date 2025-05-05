#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(IDatabase& database, LoginManager* loginManager) : m_database(database), m_loginManager(loginManager)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	return RequestResult();
}

bool MenuRequestHandler::doesUserExist(const std::string& username) const
{
	return false;
}

bool MenuRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return false;
}

bool MenuRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return false;
}
