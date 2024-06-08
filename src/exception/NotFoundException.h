#ifndef QRZ_NOTFOUNDEXCEPTION_H
#define QRZ_NOTFOUNDEXCEPTION_H

#include <exception>
#include <string>

namespace qrz
{
	/**
	 * @class AuthenticationException
	 * @brief Represents an exception that is thrown when QRZ API authentication fails.
	 *
	 * This exception class inherits from std::exception class.
	 */
	class NotFoundException : public std::exception
	{
	public:
		explicit NotFoundException(std::string_view message = "QRZ Record Not found") : m_message(message)
		{}

		const char *what() const noexcept override;

	private :
		std::string m_message;
	};
}
#endif //QRZ_NOTFOUNDEXCEPTION_H
