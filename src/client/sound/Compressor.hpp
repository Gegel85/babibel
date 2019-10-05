/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Compressor.hpp
*/
#ifndef CPP_BABEL_2019_COMPRESSOR_HPP
#define CPP_BABEL_2019_COMPRESSOR_HPP

#include <opus.h>
#include <string>
#include <vector>

#define DEFAULT_SAMPLE_RATE 48000
#define DEFAULT_CHANNELS 1
#define DEFAULT_APP OPUS_APPLICATION_VOIP
#define DEFAULT_BITRATE 64000
#define DEFAULT_FRAME_SIZE 960
#define DEFAULT_MAX_FRAME (6 * DEFAULT_FRAME_SIZE)
#define DEFAULT_MAX_PACKET_SIZE 4000

#include "CompressedAudio.hpp"

namespace Babel::Client {
    class Compressor {
    private:
        OpusEncoder *_encoder;
        OpusDecoder *_decoder;
        int _channels;
        size_t _frame_size;
        size_t _max_frame_size;
        size_t _max_packet_size;
        static float *_bytes_to_floatptr(const std::vector<float> &bytes);
        static std::vector<std::vector<float>> _split_vector(const std::vector<float> &vect, unsigned x);


    public:
        Compressor(opus_int32 sample_rate=DEFAULT_SAMPLE_RATE, int channels=DEFAULT_CHANNELS,
                   int application=DEFAULT_APP, size_t bitrate=DEFAULT_BITRATE,
                   size_t frame_size=DEFAULT_FRAME_SIZE, size_t max_frame_size=DEFAULT_MAX_FRAME,
                   size_t max_packet_size=DEFAULT_MAX_PACKET_SIZE);

        ~Compressor();
        CompressedAudio compress_bytes(const std::vector<float> &bytes) const;
        std::vector<float> uncompress_bytes(const CompressedAudio &compressed_audio) const;




    class BaseException : public std::exception {
    private:
        std::string	_msg;
    public:
        explicit BaseException(const std::string &msg) : _msg(msg) {};
        const char *what() const noexcept override { return this->_msg.c_str(); };
    };

    class CreationError : public BaseException {
    public:
        explicit CreationError(const std::string &msg) : BaseException(msg) {};
    };

    class CompressingError : public BaseException {
    public:
        explicit CompressingError(const std::string &msg) : BaseException(msg) {};
    };

    };

}


#endif //CPP_BABEL_2019_COMPRESSOR_HPP
