#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler()
{
}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return (static_cast<unsigned int>(requestInfo.id) == static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST) ||
		static_cast<unsigned int>(requestInfo.id) == static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST));
}

RequestInfo LoginRequestHandler::handleRequest(RequestInfo requestInfo)
{
	RequestInfo response;
	response.receivalTime = std::chrono::system_clock::now();

	static LoginManager loginManager; // Avoid creating it every time

	switch (static_cast<unsigned int>(requestInfo.id))
	{
	case static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST):
	{
		const LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
		const LoginResponse loginResponse{ loginManager.signIn(loginRequest.username, loginRequest.password) };

		response.id = static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST);
		response.buffer = JsonResponsePacketSerializer::serializeResponse(loginResponse);
		break;
	}
	case static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST):
	{
		const SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
		const SignupResponse signupResponse{ loginManager.signUp(signupRequest.username, signupRequest.password, signupRequest.email) };

		response.id = static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST);
		response.buffer = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		break;
	}
	default:
	{
		const ErrorResponse errorResponse{
			ResponseCode::ERROR_RESPONSE,
			"Unsupported request type"
		};

		response.id = static_cast<unsigned int>(ResponseCode::ERROR_RESPONSE);
		response.buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		break;
	}
	}

	return response;
}