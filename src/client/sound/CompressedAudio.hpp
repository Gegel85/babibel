/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** CompressedAudio.hpp
*/
#ifndef CPP_BABEL_2019_COMPRESSEDAUDIO_HPP
#define CPP_BABEL_2019_COMPRESSEDAUDIO_HPP

#include <vector>
#include <string>

namespace Babel::Client {
    class Compressor;

    class CompressedPacket {
    private:
        size_t _length;
        unsigned char *_data;

    public:
        CompressedPacket(const CompressedPacket &);
        CompressedPacket(const std::string &str);
        CompressedPacket(unsigned char *data, size_t length);
        ~CompressedPacket();
        std::string to_string() const;
        size_t get_length() const;
        unsigned char *get_raw_data() const;
    };

    class CompressedAudio {
    private:
        std::vector<CompressedPacket> _data;
        size_t _vector_length;
        static std::vector<unsigned char> _bytes_to_vector(unsigned char *data, size_t length);
    public:
        CompressedAudio(const std::vector<CompressedPacket> &data);
        CompressedAudio(const std::string &str);
        std::vector<float> uncompress_audio(const Compressor &compressor) const;
        std::string to_string() const;
        size_t get_vector_length() const;
        std::vector<CompressedPacket> get_data() const;
        void print_debug(int level=1) const;
    };
}




#endif //CPP_BABEL_2019_COMPRESSEDAUDIO_HPP
