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

		this->op = vector.at(0);
		if (length + 3 != vector.size())
			throw InvalidPacketException("Length given doesn't match");

		this->data = std::string(vector.begin() + 5, vector.end());
		return *this;
	}

	Packet::operator std::string()
	{
		if (this->data.size() >= UINT32_MAX)
			throw InvalidPacketException("Packet is too large");

		return std::string{
			static_cast<char>(this->op),
			static_cast<char>(this->data.size() >> 24U),
			static_cast<char>(this->data.size() >> 16U),
			static_cast<char>(this->data.size() >> 8U),
			static_cast<char>(this->data.size())
		} + this->data;
	}
}