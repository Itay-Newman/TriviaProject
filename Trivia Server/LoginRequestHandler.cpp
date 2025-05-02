#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory, IDatabase& database)
	: IRequestHandler(), m_LoginManager(factory.getLoginManager()), m_HandlerFactory(factory), m_database(database)
{
}

bool LoginRequestHandler::doesUserExist(const std::string& username) const
{
	return m_HandlerFactory.getDataBase().doesUserExist(username);
}

bool LoginRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_HandlerFactory.getDataBase().doesPasswordMatch(username, password);
}

bool LoginRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_HandlerFactory.getDataBase().addUser(username, password, email);
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return (requestInfo.id == static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST)) ||
		(requestInfo.id == static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST));
}

RequestInfo LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
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
