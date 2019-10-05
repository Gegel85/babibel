/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** CompressedAudio.cpp
*/

#include <cstring>
#include "CompressedAudio.hpp"
#include "Compressor.hpp"
//#include "../../network/Protocol.hpp"

namespace Babel::Client {
    CompressedAudio::CompressedAudio(const std::vector<CompressedPacket> &data):
    _data(data),
    _vector_length(data.size())
    {}

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

    void CompressedAudio::print_debug(int level) const
    {
        printf("===Compressed Audio===\nLength of vector : %lu\n", this->_vector_length);
        if (level <= 1)
            return;
        for (size_t i = 0; i < this->_data.size(); i++) {
            printf("%02lu : [", i);
            for (size_t j = 0; j < this->_data[i].get_length(); j++)
                printf("%02X, ", this->_data[i].get_raw_data()[j]);
            printf("]\n");
        }
    }


    CompressedPacket::CompressedPacket(unsigned char *data, size_t length):
    _data(data),
    _length(length)
    {}

    CompressedPacket::~CompressedPacket()
    {
        delete[] this->_data;
    }

    std::string CompressedPacket::to_string() const
    {
        return nullptr;
        //return Network::Protocol::Packet::uint32toByteString(this->_length) + reinterpret_cast<char *>(this->_data);
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