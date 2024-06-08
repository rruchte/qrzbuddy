#include "NotFoundException.h"

const char* qrz::NotFoundException::what() const noexcept
{
	return m_message.c_str();
};