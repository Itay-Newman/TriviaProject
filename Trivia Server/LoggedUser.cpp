#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& username) : m_username(username)
{
}

std::string LoggedUser::getUsername() const
{
	return this->m_username;
}

void LoggedUser::setUsername(std::string username)
{
	this->m_username = username;
}
