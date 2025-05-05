#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(IDatabase& database, LoginManager* loginManager) : m_database(database), m_loginManager(loginManager)
{
}
