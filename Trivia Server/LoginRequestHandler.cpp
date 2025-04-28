#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory)
	: m_LoginManager(loginManager), m_HandlerFactory(handlerFactory)
{
}


bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return (requestInfo.id == static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST)) ||
		(requestInfo.id == static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST));
}

RequestInfo LoginRequestHandler::handleRequest(RequestInfo requestInfo)
{
	RequestInfo response;
	response.receivalTime = std::chrono::system_clock::now();

	switch (requestInfo.id)
	{
	case static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST):
	{
		const LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
		const LoginResponse loginResponse{ m_LoginManager.signIn(loginRequest.username, loginRequest.password) };

		response.id = static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST);
		response.buffer = JsonResponsePacketSerializer::serializeResponse(loginResponse);
		break;
	}
	case static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST):
	{
		const SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
		const SignupResponse signupResponse{ m_LoginManager.signUp(signupRequest.username, signupRequest.password, signupRequest.email) };

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
