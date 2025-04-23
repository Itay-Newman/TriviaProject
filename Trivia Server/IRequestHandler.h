#pragma once

#include "Structs.h"

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(RequestInfo requestInfo) = 0;
    virtual RequestInfo handleRequest(RequestInfo requestInfo) = 0;
};