/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Protocol.cpp
*/

#include <iostream>
#include "Protocol.hpp"

namespace BabelNetwork::Protocol
{
	Packet::Packet(const std::string &vector)
	{
		*this = vector;
	}

	Packet::Packet(BabelNetwork::Socket &socket)
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
		length = uintFromByteString(socket.read(4, 2));
		key = socket.read(4, 2);
		data = socket.read(length, 2);

		this->data.clear();
		this->op ^= key.at(0);
		for (int i = 1; i <= data.size(); i++)
			this->data.push_back(data.at(i - 1) ^ key.at(i % 4));
		return *this;
	}

	Packet &Packet::operator=(const std::string &str)
	{
		unsigned int length = uintFromByteString(str.substr(1, 4));
		std::string key{str.begin() + 5, str.begin() + 9};

		this->data.clear();
		this->op = str.at(0) ^ key[0];
		if (length + 9 != str.size())
			throw InvalidPacketException("Length given doesn't match");

		for (int i = 9; i < str.size(); i++)
			this->data.push_back(str[i] ^ key[i % 4]);

		return *this;
	}


	unsigned Packet::uintFromByteString(const std::string &str)
	{
		return	static_cast<unsigned>(str.at(0) << 24U) +
			static_cast<unsigned>(str.at(1) << 16U) +
			static_cast<unsigned>(str.at(2) << 8U) +
			static_cast<unsigned char>(str.at(3));
	}

	std::string Packet::toByteString(unsigned value)
	{
		return {static_cast<char>(value >> 24U),
			static_cast<char>(value >> 16U),
			static_cast<char>(value >> 8U),
			static_cast<char>(value)};
	}

	Packet::operator std::string()
	{
		if (this->data.size() >= UINT32_MAX)
			throw InvalidPacketException("Packet is too large");

		std::string codedData;
		std::string keyStr = toByteString(this->_random());

		for (int i = 1; i <= this->data.size(); i++)
			codedData.push_back(this->data.at(i - 1) ^ keyStr[i % 4]);

		return std::string{
			static_cast<char>(this->op ^ keyStr[0])
		} + toByteString(this->data.size()) + keyStr + codedData;
	}

	std::string ErrorReason::NORMAL_CLOSURE =	{'\0', '\0', '\0', '\x00'};
	std::string ErrorReason::BAD_PACKET =		{'\0', '\0', '\0', '\x01'};
	std::string ErrorReason::BAD_VERSION =		{'\0', '\0', '\0', '\x02'};
	std::string ErrorReason::BAD_OPCODE =		{'\0', '\0', '\0', '\x03'};
	std::string ErrorReason::BAD_CREDENTIALS =	{'\0', '\0', '\0', '\x04'};
	std::string ErrorReason::ALREADY_CONNECTED =	{'\0', '\0', '\0', '\x05'};
	std::string ErrorReason::ALREADY_USED =		{'\0', '\0', '\0', '\x06'};
	std::string ErrorReason::NOT_CONNECTED =	{'\0', '\0', '\0', '\x07'};
	std::string ErrorReason::NOT_AUTHORIZED =	{'\0', '\0', '\0', '\x08'};
	std::string ErrorReason::NOT_FOUND =		{'\0', '\0', '\0', '\x09'};
	std::string ErrorReason::REMOTE_NOT_CONNECTED =	{'\0', '\0', '\0', '\x0A'};
}

std::ostream &operator<<(std::ostream &stream, BabelNetwork::Protocol::Packet &packet)
{
	stream << "op: " << static_cast<unsigned>(packet.op);
	stream << " data: [";
	for (int i = 0; i < packet.data.size(); i++) {
		if (i)
			stream << ", ";
		stream << static_cast<unsigned>(static_cast<unsigned char>(packet.data[i]));
	}
	stream << "] (" << packet.data.size() << "B)";
	return stream;
}