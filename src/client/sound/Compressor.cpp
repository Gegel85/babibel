/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Compressor.cpp
*/
#include <iostream>
#include "Compressor.hpp"

namespace Babel::Client
{
    Compressor::Compressor(opus_int32 sample_rate, int channels, int application, size_t bitrate, size_t frame_size,
                           size_t max_frame_size, size_t max_packet_size)
    {
        int error;

        this->_channels = channels;
        this->_frame_size = frame_size;
        this->_max_frame_size = max_frame_size;
        this->_max_packet_size = max_packet_size;
        this->_encoder = nullptr;
        this->_decoder = nullptr;

        this->_encoder = opus_encoder_create(sample_rate, channels, application, &error);
        if (error < 0) {
            std::cerr << "An error has occured durring the creation of encoder: " << opus_strerror(error);
            throw CreationError("Error has occured durring the creation of encoder");
        }

        error = opus_encoder_ctl(this->_encoder, OPUS_SET_BITRATE(bitrate));
        if (error < 0) {
            opus_encoder_destroy(this->_encoder);
            std::cerr << "An error has occured durring setting bitrate of encoder: " << opus_strerror(error);
            throw CreationError("An error has occured durring setting bitrate of encoder");
        }

        this->_decoder = opus_decoder_create(sample_rate, channels, &error);
        if (error < 0) {
            opus_encoder_destroy(this->_encoder);
            std::cerr << "An error has occured durring the creation of decoder: " << opus_strerror(error);
            throw CreationError("An error has occured durring the creation of decoder");
        }

    }

    Compressor::~Compressor()
    {
        if (this->_encoder)
            opus_encoder_destroy(this->_encoder);
        if (this->_decoder)
            opus_decoder_destroy(this->_decoder);
    }

    opus_int16 *Compressor::bytes_to_short(const std::vector<unsigned char> &bytes)
    {
        std::vector<unsigned char> src = bytes;
        if (src.size() % 2)
            src.push_back(0);

        auto *result = new opus_int16[bytes.size() / 2];

        for (int i = 0; i < src.size() / 2; i++)
            result[i] = (src[i * 2 + 1] << 8U) + src[i * 2];
        return result;
    }

    std::vector<unsigned char> Compressor::compress_bytes(const std::vector<unsigned char> &bytes) const
    {
        opus_int16 *input_data = this->bytes_to_short(bytes);
        auto *output_data = new unsigned char[this->_max_packet_size];
        int nb = opus_encode(this->_encoder, input_data, this->_frame_size, output_data, this->_max_packet_size);

        if (nb < 0) {
            std::cerr << "An error has occured durring encoding: " << opus_strerror(nb);
            throw CompressingError("An error occured durring encoding");
        }

        std::vector<unsigned char> result{static_cast<size_t>(nb), 0, std::vector<unsigned char>::allocator_type()};
        for (int i = 0; i < nb; i++)
            result[i] = output_data[i];
        delete[] input_data;
        delete[] output_data;
        return result;
    }

    std::vector<unsigned char> Compressor::uncompress_bytes(const std::vector<unsigned char> &bytes) const
    {
        int nb = bytes.size();
        auto *data = new unsigned char [bytes.size()];
        auto *out = new opus_int16[this->_max_frame_size * this->_channels];

        for (int i = 0; i < nb; i++)
            data[i] = bytes[i];
        int frame_size = opus_decode(this->_decoder, data, nb, out, this->_max_frame_size, 0);
        if (frame_size<0) {
            std::cerr << "An error has occured durring decoding: " << opus_strerror(nb);
            throw CompressingError("An error occured durring decoding");
        }

        std::vector<unsigned char> result{static_cast<size_t>(this->_channels * frame_size), 0, std::vector<unsigned char>::allocator_type()};
        for(int i = 0; i < this->_channels * frame_size; i++) {
            result[2*i] = out[i] & 0xFF;
            result[2*i+1] = (out[i]>>8) & 0xFF;
        }
        delete[] data;
        delete[] out;
        return result;
    }
}