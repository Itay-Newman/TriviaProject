#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory, IDatabase& database)
	: IRequestHandler(), m_loginManager(factory.getLoginManager()), m_handlerFactory(factory), m_database(database)
{
}

bool LoginRequestHandler::doesUserExist(const std::string& username) const
{
	return m_database.doesUserExist(username);
}

bool LoginRequestHandler::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return m_database.doesPasswordMatch(username, password);
}

bool LoginRequestHandler::addUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return m_database.addUser(username, password, email);
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return (requestInfo.id == static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST)) ||
		(requestInfo.id == static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST));
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult response;

	switch (requestInfo.id)
	{
	case static_cast<unsigned int>(RequestCodes::LOGIN_REQUEST):
	{
		const LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
		const LoginResponse loginResponse{ m_loginManager.signIn(loginRequest.username, loginRequest.password) };

		response.response = JsonResponsePacketSerializer::serializeResponse(loginResponse);
		response.id = ResponseCode::LOGIN_RESPONSE;

		// Assuming status 1 = success, 0 = failure
		if (loginResponse.status == (int)Status::SUCCESS)
		{
			response.newHandler = new MenuRequestHandler(m_handlerFactory.getDataBase(), &m_handlerFactory.getLoginManager(), &m_handlerFactory.getRoomManager(), &m_handlerFactory.getStatisticsManager(), loginRequest.username);
		}
		else
		{
			response.newHandler = nullptr;
		}
		break;
	}
	case static_cast<unsigned int>(RequestCodes::SIGNUP_REQUEST):
	{
		const SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
		const SignupResponse signupResponse{ m_loginManager.signUp(signupRequest.username, signupRequest.password, signupRequest.email) };

		response.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		response.id = ResponseCode::SIGNUP_RESPONSE;

		if (signupResponse.status == (int)Status::SUCCESS)
		{
			response.newHandler = new MenuRequestHandler(m_handlerFactory.getDataBase(), &m_handlerFactory.getLoginManager(), &m_handlerFactory.getRoomManager(), &m_handlerFactory.getStatisticsManager(), signupRequest.username);
		}
		else
		{
			response.newHandler = nullptr;
		}
		break;
	}
	default:
	{
		const ErrorResponse errorResponse{
			ResponseCode::ERROR_RESPONSE,
			"Unsupported request type"
		};
		response.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		response.id = ResponseCode::ERROR_RESPONSE;
		response.newHandler = nullptr;
		break;
	}
	}

	return response;
}
