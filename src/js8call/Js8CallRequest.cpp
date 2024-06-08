#include "Js8CallRequest.h"

Js8CallRequest::Js8CallRequest(Js8CallRequestType type, Js8CallResponseCallback callback) : m_type(type), m_callback(callback)
{
}

Js8CallRequestType Js8CallRequest::getType()
{
	return m_type;
}

void Js8CallRequest::handleResponse(const std::string &responseValue)
{
	m_callback(responseValue);
}

Js8CallResponseCallback Js8CallRequest::getCallback()
{
	return m_callback;
}
