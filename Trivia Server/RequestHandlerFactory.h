#pragma once

#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
public:
    static IRequestHandler* createHandler(int requestCode);
};