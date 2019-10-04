
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "Sound.hpp"

namespace Babel::Client::Sound
{
	Sound::Sound()
	{
		PaError err = Pa_Initialize();

		if (err != paNoError)
			throw Exceptions::InitializationErrorException(Pa_GetErrorText(err));
	}

	Sound::~Sound()
	{

	}

	void Sound::playBuffer(const std::vector<float> &buffer, size_t sampleRate, unsigned channelCount)
	{
		PaError		err;
		SoundState	state;
		PaStream	*stream;
		PaStreamParameters params;

		if (!channelCount)
			throw Exceptions::InvalidChannelCountException("There must be at least 1 channel");

		state.channelCount = channelCount;
		state.currentIndex = 0;
		state.buffer = buffer;

		params.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (params.device == paNoDevice)
			throw Exceptions::NoOutputDeviceException("No default output device.");

		params.channelCount = channelCount;
		params.sampleFormat = paFloat32;
		params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
		params.hostApiSpecificStreamInfo = nullptr;

		err = Pa_OpenStream(
			&stream,
			nullptr,
			&params,
			sampleRate,
			FRAMES_PER_BUFFER,
			paClipOff,
			Sound::_playCallback,
			&state
		);
		if (err != paNoError)
			throw Exceptions::OpenStreamErrorException(std::string("Couldn't open stream: ") + Pa_GetErrorText(err));

		if (!stream)
			return;

		err = Pa_StartStream(stream);
		if (err != paNoError)
			throw Exceptions::StartStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));

		for (err = Pa_IsStreamActive(stream); err == 1; err = Pa_IsStreamActive(stream))
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (err < 0)
			throw Exceptions::StreamActivityCheckErrorException(std::string("Couldn't check stream's activity: ") + Pa_GetErrorText(err));

		err = Pa_CloseStream(stream);
		if (err != paNoError)
			throw Exceptions::CloseStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));

	}

	std::vector<float> Sound::recordAudio(float duration, size_t sampleRate, unsigned channelCount)
	{
		PaError		err;
		SoundState	state;
		PaStream	*stream;
		PaStreamParameters params;

		if (!channelCount)
			throw Exceptions::InvalidChannelCountException("There must be at least 1 channel");

		state.channelCount = channelCount;
		state.currentIndex = 0;
		state.buffer.resize(std::ceil(duration * sampleRate * channelCount), 0);

		params.device = Pa_GetDefaultInputDevice();
		if (params.device == paNoDevice)
			throw Exceptions::NoInputDeviceException("No default input device");

		params.channelCount = channelCount;
		params.sampleFormat = paFloat32;
		params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowInputLatency;
		params.hostApiSpecificStreamInfo = nullptr;
		err = Pa_OpenStream(
			&stream,
			&params,
			nullptr,
			sampleRate,
			FRAMES_PER_BUFFER,
			paClipOff,
			Sound::_recordCallback,
			&state
		);
		if (err != paNoError)
			throw Exceptions::OpenStreamErrorException(std::string("Couldn't open stream: ") + Pa_GetErrorText(err));

		err = Pa_StartStream(stream);
		if (err != paNoError)
			throw Exceptions::StartStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));


		for (err = Pa_IsStreamActive(stream); err == 1; err = Pa_IsStreamActive(stream))
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (err < 0)
			throw Exceptions::StreamActivityCheckErrorException(std::string("Couldn't check stream's activity: ") + Pa_GetErrorText(err));

		err = Pa_CloseStream(stream);
		if (err != paNoError)
			throw Exceptions::CloseStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));

		return state.buffer;
	}

	int Sound::_recordCallback(
		const void *inputBuffer,
		void *,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo *,
		PaStreamCallbackFlags,
		void *userData
	)
	{
		SoundState &state = *reinterpret_cast<SoundState *>(userData);

		framesPerBuffer *= state.channelCount;

		unsigned int index = state.currentIndex;
		auto *input = reinterpret_cast<const float *>(inputBuffer);
		unsigned long framesLeft = state.buffer.size() - state.currentIndex;
		long framesToCalc;

		framesToCalc = framesLeft < framesPerBuffer ? framesLeft : framesPerBuffer;

		if (inputBuffer)
			for (unsigned i = index; i < framesToCalc + index; i += state.channelCount)
				for (unsigned j = i; j < i + state.channelCount; j++)
					state.buffer[j] = *input++;

		state.currentIndex += framesToCalc;
		return framesLeft < framesPerBuffer ? paComplete : paContinue;
	}

	int Sound::_playCallback(
		const void *,
		void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo *,
		PaStreamCallbackFlags ,
		void *userData
	)
	{
		SoundState &state = *reinterpret_cast<SoundState *>(userData);
		unsigned int index = state.currentIndex;
		auto *output = reinterpret_cast<float *>(outputBuffer);
		unsigned long framesLeft = state.buffer.size() - state.currentIndex;

		framesPerBuffer *= state.channelCount;

		if (framesLeft < framesPerBuffer) {
			for (unsigned i = index; i < framesLeft + index; i += state.channelCount)
				for (unsigned j = i; j < i + state.channelCount; j++)
					*output++ = state.buffer[j];
			for (unsigned i = framesLeft + index; i < framesPerBuffer + index; i += state.channelCount)
				for (unsigned j = i; j < i + state.channelCount; j++)
					*output++ = 0;
			state.currentIndex += framesLeft;
			return paComplete;
		}

		for (unsigned i = index; i < framesPerBuffer + index;  i += state.channelCount)
			for (unsigned j = i; j < i + state.channelCount; j++)
				*output++ = state.buffer[j];

		state.currentIndex += framesPerBuffer;
		return paContinue;
	}
}
