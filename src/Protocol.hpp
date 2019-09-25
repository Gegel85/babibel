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

namespace Babel::Protocol
{
	enum Opcode {
		HELLO,
		BYE,
		REGISTER,
		GET_FRIENDS,
		GET_USER_INFOS,
		CALL,
		CALL_ACCEPTED,
		CALL_REFUSED,
	};

	namespace ExitReason {
		extern std::string NORMAL_CLOSURE;
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
		Packet(const std::vector<unsigned char> &vector);
		Packet &operator=(const std::vector<unsigned char> &vector);
		operator std::string();
	};
}


#endif //CPP_BABEL_2019_PROTOCOL_HPP
