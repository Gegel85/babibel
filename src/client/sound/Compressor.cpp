/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Compressor.cpp
*/
#include <iostream>
#include <cmath>
#include "Compressor.hpp"

namespace Babel::Client
{
    Compressor::Compressor(opus_int32 sample_rate, int channels, int application, size_t bitrate, size_t frame_size,
                           size_t max_frame_size, size_t max_packet_size):
                           _encoder(nullptr),
                           _decoder(nullptr),
                           _channels(channels),
                           _frame_size(frame_size),
                           _max_frame_size(max_frame_size),
                           _max_packet_size(max_packet_size)
    {
        int error;

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

    float *Compressor::_bytes_to_floatptr(const std::vector<float> &bytes)
    {
        auto *result = new float[bytes.size()];

        for (unsigned long i = 0; i < bytes.size(); i++)
            result[i] = bytes[i];
        return result;
    }

    std::vector<std::vector<float>> Compressor::_split_vector(const std::vector<float> &vect, unsigned x)
    {
        int size = (vect.size() - 1) / x + 1;

        std::vector<std::vector<float>> result;

        result.resize(size);

        for (int i = 0; i < size; i++) {
            auto start_itr = std::next(vect.cbegin(), i * x);
            auto end_itr = std::next(vect.cbegin(), i * x + x);

            result[i].resize(x);

            if (i * x + x > vect.size()) {
                end_itr = vect.cend();
                result[i].resize(vect.size() - i * x);
            }
            std::copy(start_itr, end_itr, result[i].begin());
        }
        return result;
    }

    CompressedAudio Compressor::compress_bytes(const std::vector<float> &bytes) const
    {
        auto buffer = bytes;
        buffer.resize(static_cast<unsigned long>(std::ceil(static_cast<float>(buffer.size()) / this->_frame_size) * this->_frame_size), 0);

        std::vector<std::vector<float>> buffers = this->_split_vector(buffer, this->_frame_size * this->_channels);
        std::vector<CompressedPacket> result = {};

        for (auto &chuncked : buffers) {
            float *input_data = this->_bytes_to_floatptr(chuncked);
            auto *output_data = new unsigned char[this->_max_packet_size];
            int nb = opus_encode_float(this->_encoder, input_data, this->_frame_size, output_data,
                                       this->_max_packet_size);
            delete[] input_data;
            if (nb < 0)
                throw CompressingError("An error occured durring encoding" + std::string(opus_strerror(nb)));
            result.emplace_back(output_data, nb);
        }

        return {result};
    }

    std::vector<float> Compressor::uncompress_bytes(const CompressedAudio &ca) const
    {

        std::vector<float> result;

        for (auto &chuncked : ca.get_data()) {
            auto *out = new float[this->_frame_size * this->_channels];
            int dec_samples = opus_decode_float(this->_decoder, chuncked.get_raw_data(), chuncked.get_length(),
                                                out, this->_frame_size, 0);
            if (dec_samples < 0) {
                delete[] out;
                std::cerr << "An error has occured durring decoding: " << opus_strerror(dec_samples);
                throw CompressingError("An error occured durring decoding");
            }

            for (int i = 0; i < dec_samples; i++)
                result.push_back(out[i]);
            delete[] out;
        }
        return result;
    }
}