//
// Created by rob on 5/29/24.
//

#ifndef QRZBUDDY_JS8CALLREQUEST_H
#define QRZBUDDY_JS8CALLREQUEST_H

#include <string>
#include <functional>
#include "Js8CallRequestType.h"

typedef std::function<void(const std::string &value)> Js8CallResponseCallback;

class Js8CallRequest{
public:
	Js8CallRequest(Js8CallRequestType type, Js8CallResponseCallback callback);
	~Js8CallRequest() = default;
	Js8CallRequestType getType();
	Js8CallResponseCallback getCallback();
	void handleResponse(const std::string &responseValue);

private:
	Js8CallRequestType m_type;
	Js8CallResponseCallback m_callback;
};

#endif //QRZBUDDY_JS8CALLREQUEST_H
