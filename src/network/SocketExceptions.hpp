/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** SocketExceptions.hpp
*/

#ifndef CPP_BABEL_2019_SOCKETEXCEPTIONS_HPP
#define CPP_BABEL_2019_SOCKETEXCEPTIONS_HPP


#include <exception>
#include <string>

namespace Babel::Network::Exceptions
{
	class BaseException : public std::exception {
	private:
		std::string	_msg;
	public:
		explicit BaseException(const std::string &msg) : _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class TimeoutException : public BaseException {
	public:
		explicit TimeoutException(const std::string &msg) : BaseException(msg) {};
	};

	class SocketCreationErrorException : public BaseException {
	public:
		explicit SocketCreationErrorException(const std::string &msg) : BaseException(msg) {};
	};

	class HostNotFoundException : public BaseException {
	public:
		explicit HostNotFoundException(const std::string &msg) : BaseException(msg) {};
	};

	class ConnectException : public BaseException {
	public:
		explicit ConnectException(const std::string &msg) : BaseException(msg) {};
	};

	class NotConnectedException : public BaseException {
	public:
		explicit NotConnectedException(const std::string &msg) : BaseException(msg) {};
	};

	class AlreadyOpenedException : public BaseException {
	public:
		explicit AlreadyOpenedException(const std::string &msg) : BaseException(msg) {};
	};

	class EOFException : public BaseException {
	public:
		explicit EOFException(const std::string &msg) : BaseException(msg) {};
	};

	class BindFailedException : public BaseException {
	public:
		explicit BindFailedException(const std::string &msg) : BaseException(msg) {};
	};

	class ListenFailedException : public BaseException {
	public:
		explicit ListenFailedException(const std::string &msg) : BaseException(msg) {};
	};

	class InvalidHandlerException : public BaseException {
	public:
		explicit InvalidHandlerException(const std::string &msg) : BaseException(msg) {};
	};
}


#endif //CPP_BABEL_2019_SOCKETEXCEPTIONS_HPP
