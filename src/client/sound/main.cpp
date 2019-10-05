/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main.cpp
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Compressor.hpp"
#include "Sound.hpp"

int main()
{
    Babel::Client::Sound::Sound sound;
    Babel::Client::Compressor compressor;
    std::cout << "Record" << std::endl;
    std::vector<float> meilleur = sound.recordAudio(3, 48000, 1);

    std::cout << "Meilleur before " << meilleur.size() << std::endl;
    std::cout << "Compress" << std::endl;
    Babel::Client::CompressedAudio ac = compressor.compress_bytes(meilleur);

    ac.print_debug(1);

    meilleur = ac.uncompress_audio(compressor);

    std::cout << "Meilleur after " << meilleur.size() << std::endl;
    std::cout << "Play" << std::endl;
    sound.playBuffer(meilleur, 48000, 1);
    while (!sound.isReady())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}