#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
    // Checking if the message code is a Login or SignUp request
    return (requestInfo.id == RequestCodes::LOGIN_REQUEST ||
        requestInfo.id == RequestCodes::SIGNUP_REQUEST);
}

RequestInfo LoginRequestHandler::handleRequest(RequestInfo requestInfo)
{
    RequestInfo response;
    response.receivalTime = std::chrono::system_clock::now();

    if (requestInfo.id == RequestCodes::LOGIN_REQUEST)
    {
        // Deserialize login request
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);

        // Process login...
        // For now, I'll just create a simple response
        LoginResponse loginResponse;
        loginResponse.status = 1; // Success

        // Serialize the response
        std::vector<unsigned char> serializedResponse = JsonResponsePacketSerializer::serializeResponse(loginResponse);

        // Set response data
        response.id = 1; // Login response code
        response.buffer = serializedResponse;
    }
    
    else if (requestInfo.id == RequestCodes::SIGNUP_REQUEST)
    {
        // Deserialize signup request
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);

        // Process signup...
        // For now, I'll just create a simple response just like the login
        SignupResponse signupResponse;
        signupResponse.status = 1;

        std::vector<unsigned char> serializedResponse = JsonResponsePacketSerializer::serializeResponse(signupResponse);

        response.id = 2;
        response.buffer = serializedResponse;
    }
    
    else
    {
        // Creating an error response for unsupported request types
        ErrorResponse errorResponse;
        errorResponse.status = 0; // Fail
        errorResponse.message = "Unsupported request type";

        std::vector<unsigned char> serializedResponse = JsonResponsePacketSerializer::serializeResponse(errorResponse);

        response.id = 0; // Error response code
        response.buffer = serializedResponse;
    }

    return response;
}