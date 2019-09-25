/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Protocol.cpp
*/

#include "Protocol.hpp"

namespace Babel::Protocol
{
	Packet::Packet(const std::vector<unsigned char> &vector)
	{
		*this = vector;
	}

	Packet &Packet::operator=(const std::vector<unsigned char> &vector)
	{
		unsigned int length =
			(vector.at(1) << 24U) +
			(vector.at(2) << 16U) +
			(vector.at(3) << 8U) +
			vector.at(4);
		std::string key{vector.begin() + 5, vector.begin() + 9};

		this->data.clear();
		this->op = vector.at(0);
		if (length + 9 != vector.size())
			throw InvalidPacketException("Length given doesn't match");

		for (int i = 9; i < vector.size(); i++)
			this->data.push_back(vector[i] ^ key[i % 4]);

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
}