#pragma once

#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler();
	~LoginRequestHandler();

	// Virtual functions implementation
	virtual bool isRequestRelevant(RequestInfo requestInfo) override;
	virtual RequestInfo handleRequest(RequestInfo requestInfo) override;
};