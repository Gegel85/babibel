
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "Sound.hpp"

namespace Babel::Client::Sound
{
	Sound::Sound()
	{
		this->_state.stopped = true;
		PaError err = Pa_Initialize();

		if (err != paNoError)
			throw Exceptions::InitializationErrorException(Pa_GetErrorText(err));
	}

	Sound::~Sound()
	{
		this->_state.stopped = true;
		if (this->_mainThread.joinable())
			this->_mainThread.join();
	}

	std::vector<float> Sound::recordAudio(float duration, size_t sampleRate, unsigned int channelCount)
	{
		if (!this->isReady())
			throw Exceptions::AlreadyWorkingException("This sound is still working");

		this->_state.buffer.resize(std::ceil(duration * sampleRate * channelCount), 0);
		this->_record(_recordBufferCallback, sampleRate, channelCount);
		return this->_state.buffer;
	}

	void Sound::playBuffer(const std::vector<float> &buffer, size_t sampleRate, unsigned int channelCount)
	{
		if (!this->isReady())
			throw Exceptions::AlreadyWorkingException("This sound is still working");

		this->_state.buffer = buffer;
		this->_play(_playBufferCallback, sampleRate, channelCount);
	}

	bool Sound::isReady() const
	{
		return this->_state.stopped;
	}

	void Sound::_play(const PortAudioHandler &handler, size_t sampleRate, unsigned channelCount)
	{
		this->_state.handler = handler;
		this->_setupStream(sampleRate, channelCount, false, true);
	}

	void Sound::_record(const Babel::Client::Sound::Sound::PortAudioHandler &handler, size_t sampleRate, unsigned channelCount)
	{
		this->_state.handler = handler;
		this->_setupStream(sampleRate, channelCount, true, false);
		this->_mainThread.join();
	}

	void Sound::_setupStream(size_t sampleRate, unsigned channelCount, bool input, bool output)
	{
		PaError		err;
		PaStream	*stream;
		SoundState	&state = this->_state;
		PaStreamParameters inparams;
		PaStreamParameters outparams;

		if (!channelCount)
			throw Exceptions::InvalidChannelCountException("There must be at least 1 channel");

		if (this->_mainThread.joinable())
			this->_mainThread.join();

		state.stopped = false;
		state.channelCount = channelCount;
		state.currentIndex = 0;

		if (input) {
			inparams.device = Pa_GetDefaultInputDevice();
			if (inparams.device == paNoDevice)
				throw Exceptions::NoInputDeviceException("No default input device");

			inparams.channelCount = channelCount;
			inparams.sampleFormat = paFloat32;
			inparams.suggestedLatency = Pa_GetDeviceInfo(inparams.device)->defaultLowInputLatency;
			inparams.hostApiSpecificStreamInfo = nullptr;
		}
		if (output) {
			outparams.device = Pa_GetDefaultOutputDevice(); /* default output device */
			if (outparams.device == paNoDevice)
				throw Exceptions::NoOutputDeviceException("No default output device.");

			outparams.channelCount = channelCount;
			outparams.sampleFormat = paFloat32;
			outparams.suggestedLatency = Pa_GetDeviceInfo(outparams.device)->defaultLowOutputLatency;
			outparams.hostApiSpecificStreamInfo = nullptr;
		}

		err = Pa_OpenStream(
			&stream,
			input ? &inparams : nullptr,
			output ? &outparams : nullptr,
			sampleRate,
			FRAMES_PER_BUFFER,
			paClipOff,
			Sound::_defaultCallback,
			this
		);
		if (err != paNoError)
			throw Exceptions::OpenStreamErrorException(std::string("Couldn't open stream: ") + Pa_GetErrorText(err));

		if (!stream)
			return;

		err = Pa_StartStream(stream);
		if (err != paNoError)
			throw Exceptions::StartStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));

		this->_mainThread = std::thread{[stream](){
			try {
				PaError	err;

				for (err = Pa_IsStreamActive(stream); err == 1; err = Pa_IsStreamActive(stream))
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

				if (err < 0)
					throw Exceptions::StreamActivityCheckErrorException(std::string("Couldn't check stream's activity: ") + Pa_GetErrorText(err));

				err = Pa_CloseStream(stream);
				if (err != paNoError)
					throw Exceptions::CloseStreamErrorException(std::string("Couldn't close stream: ") + Pa_GetErrorText(err));
			} catch (std::exception &e) {
				std::cerr << "Error in playing thread: " << e.what() << std::endl;
			}
		}};
	}

	Sound::PortAudioHandler Sound::_recordBufferCallback{
		[](
			const float *input,
			float *,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *,
			PaStreamCallbackFlags ,
			SoundState &state
		)
		{
			framesPerBuffer *= state.channelCount;

			unsigned int index = state.currentIndex;
			unsigned long framesLeft = state.buffer.size() - state.currentIndex;
			long framesToCalc;

			framesToCalc = framesLeft < framesPerBuffer ? framesLeft : framesPerBuffer;

			if (input)
				for (unsigned i = index; i < framesToCalc + index; i += state.channelCount)
					for (unsigned j = i; j < i + state.channelCount; j++)
						state.buffer[j] = *input++;

			state.currentIndex += framesToCalc;
			state.stopped = framesLeft < framesPerBuffer;
			return static_cast<int>(framesLeft < framesPerBuffer ? paComplete : paContinue);
		}
	};

	Sound::PortAudioHandler Sound::_playBufferCallback{
		[](
			const float *,
			float *output,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *,
			PaStreamCallbackFlags ,
			SoundState &state
		)
		{
			unsigned int index = state.currentIndex;
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
				state.stopped = true;
				return static_cast<int>(paComplete);
			}

			for (unsigned i = index; i < framesPerBuffer + index;  i += state.channelCount)
				for (unsigned j = i; j < i + state.channelCount; j++)
					*output++ = state.buffer[j];

			state.currentIndex += framesPerBuffer;
			return static_cast<int>(paContinue);
		}
	};

	int Sound::_defaultCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		SoundState &data = *reinterpret_cast<SoundState *>(userData);

		return data.handler(
			reinterpret_cast<const float *>(inputBuffer),
			reinterpret_cast<float *>(outputBuffer),
			framesPerBuffer,
			timeInfo,
			statusFlags,
			data
		);
	}
}
