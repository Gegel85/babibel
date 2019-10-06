/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** CompressedAudio.cpp
*/

#include <cstring>
#include "CompressedAudio.hpp"
#include "Compressor.hpp"
#include "../../network/Protocol.hpp"

namespace Babel::Client {
    CompressedAudio::CompressedAudio(const std::vector<CompressedPacket> &data):
    _data(data),
    _vector_length(data.size())
    {}

    CompressedAudio::CompressedAudio(const std::string &strs)
    {
        std::string str = strs;

        while (!str.empty()) {
            unsigned length = Network::Protocol::Packet::uint32FromByteString(str);

            this->_data.emplace_back(str);
            str = str.substr(4 + length);
        }
    }

    std::vector<unsigned char> CompressedAudio::_bytes_to_vector(unsigned char *data, size_t length)
    {
        std::vector<unsigned char> result = {};

        for (size_t i = 0; i < length; i++)
            result.push_back(data[i]);
        return result;
    }

    std::string CompressedAudio::to_string() const
    {
        std::string str;
        for (const CompressedPacket &pck : this->_data)
            str += pck.to_string();
        return str;
    }

    size_t CompressedAudio::get_vector_length() const
    {
        return this->_vector_length;
    }

    std::vector<CompressedPacket> CompressedAudio::get_data() const
    {
        return this->_data;
    }

    std::vector<float> CompressedAudio::uncompress_audio(const Compressor &compressor) const
    {
        return compressor.uncompress_bytes(*this);
    }

    CompressedPacket::CompressedPacket(unsigned char *data, size_t length):
    _length(length),
    _data(data)
    {}

    CompressedPacket::CompressedPacket(const std::string &str) :
        _length{Network::Protocol::Packet::uint32FromByteString(str)},
        _data{new unsigned char[this->_length]}
    {
        std::memcpy(this->_data, &str.c_str()[4], this->_length);
    }

    CompressedPacket::~CompressedPacket()
    {
        delete[] this->_data;
    }

    std::string CompressedPacket::to_string() const
    {
        return Network::Protocol::Packet::uint32toByteString(this->_length) + reinterpret_cast<char *>(this->_data);
    }

    size_t CompressedPacket::get_length() const
    {
        return this->_length;
    }

    unsigned char* CompressedPacket::get_raw_data() const
    {
        return this->_data;
    }

    CompressedPacket::CompressedPacket(const Babel::Client::CompressedPacket &other)
    {
        this->_data = new unsigned char[other.get_length()];
        this->_length = other.get_length();
        std::memcpy(this->_data, other.get_raw_data(), this->_length);
    }
}