/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Protocol.hpp
*/

#ifndef CPP_BABEL_2019_PROTOCOL_HPP
#define CPP_BABEL_2019_PROTOCOL_HPP


#include <vector>
#include <random>
#include <string>
#include "Socket.hpp"

#define VERSION_STR std::string{'\0', '\x01'}

namespace Babel::Protocol
{
	enum Opcode {
		OK,
		KO,
		HELLO,
		BYE,
		CONNECT,
		REGISTER,
		GET_FRIENDS,
		GET_USER_INFOS,
		CALL,
		CALL_ACCEPTED,
		CALL_REFUSED,
	};

	namespace ExitReason {
		extern std::string NORMAL_CLOSURE;
		extern std::string BAD_PACKET;
		extern std::string BAD_VERSION;
		extern std::string INVALID_OPCODE;
	}

	class InvalidPacketException : public std::exception {
	private:
		std::string _msg;

	public:
		InvalidPacketException(const std::string &msg) : _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};

	class Packet {
	private:
		std::random_device _random;

	public:
		unsigned char op;
		std::string data;

		Packet() = default;
		Packet(Socket &);
		Packet(const std::string &);
		Packet &operator=(Socket &);
		Packet &operator=(const std::string &);
		operator std::string();
	};
}


#endif //CPP_BABEL_2019_PROTOCOL_HPP
