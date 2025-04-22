#include "RequestHandlerFactory.h"

IRequestHandler* RequestHandlerFactory::createHandler(int requestCode)
{
    // Checking the request code and creating the appropriate handler
    if (requestCode == RequestCodes::LOGIN_REQUEST || requestCode == RequestCodes::SIGNUP_REQUEST)
    {
        return new LoginRequestHandler();
    }

    // Default handler or returning nullptr for unknown request types
    return nullptr;
}