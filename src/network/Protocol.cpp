/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Protocol.cpp
*/

#include "Protocol.hpp"

namespace Babel::Protocol
{
	Packet::Packet(const std::string &vector)
	{
		*this = vector;
	}

	Packet::Packet(Babel::Socket &socket)
	{
		*this = socket;
	}

	Packet &Packet::operator=(Socket &socket)
	{
		unsigned length;
		std::string lenStr;
		std::string key;
		std::string data;

		this->op = socket.read(1, 2).at(0);
		key = socket.read(4, 2);
		lenStr = socket.read(4, 2);
		length= static_cast<unsigned char>(lenStr[0] << 24U) +
			static_cast<unsigned char>(lenStr[1] << 16U) +
			static_cast<unsigned char>(lenStr[2] << 8U) +
			static_cast<unsigned char>(lenStr[3]);
		data = socket.read(length, 2);

		this->data.clear();
		this->op ^= key.at(0);
		for (int i = 1; i < data.size(); i++)
			this->data.push_back(data.at(i - 1) ^ key.at(i % 4));
		return *this;
	}

	Packet &Packet::operator=(const std::string &str)
	{
		unsigned int length =
			static_cast<unsigned char>(str.at(1) << 24U) +
			static_cast<unsigned char>(str.at(2) << 16U) +
			static_cast<unsigned char>(str.at(3) << 8U) +
			static_cast<unsigned char>(str.at(4));
		std::string key{str.begin() + 5, str.begin() + 9};

		this->data.clear();
		this->op = str.at(0) ^ key[0];
		if (length + 9 != str.size())
			throw InvalidPacketException("Length given doesn't match");

		for (int i = 9; i < str.size(); i++)
			this->data.push_back(str[i] ^ key[i % 4]);

		return *this;
	}

	Packet::operator std::string()
	{
		if (this->data.size() >= UINT32_MAX)
			throw InvalidPacketException("Packet is too large");

		unsigned int key = this->_random();
		std::string data;
		std::string keyStr = {
			static_cast<char>(key >> 24U),
			static_cast<char>(key >> 16U),
			static_cast<char>(key >> 8U),
			static_cast<char>(key)
		};

		for (int i = 1; i < this->data.size(); i++)
			data.push_back(this->data[i - 1] ^ keyStr[i % 4]);

		return std::string{
			static_cast<char>(this->op ^ keyStr[0]),
			static_cast<char>(this->data.size() >> 24U),
			static_cast<char>(this->data.size() >> 16U),
			static_cast<char>(this->data.size() >> 8U),
			static_cast<char>(this->data.size())
		} + keyStr + data;
	}

	std::string ExitReason::NORMAL_CLOSURE = {'\0', '\0', '\0', '\0'};
	std::string ExitReason::BAD_PACKET = {'\0', '\0', '\0', '\x01'};
	std::string ExitReason::BAD_VERSION = {'\0', '\0', '\0', '\x02'};
	std::string ExitReason::INVALID_OPCODE = {'\0', '\0', '\0', '\x03'};
}