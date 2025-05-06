#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& username) : username(username)
{
}

std::string LoggedUser::getUsername() const
{
	return this->username;
}
