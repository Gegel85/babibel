#include <vector>
#include <string>
#include <functional>
#include <thread>
#include <portaudio.h>
#include "../../network/Socket.hpp"


#ifndef BABEL_SOUND_HPP
#define BABEL_SOUND_HPP


#define FRAMES_PER_BUFFER (512)

namespace Babel::Client::Sound
{
	namespace Exceptions {
		class BaseException : public std::exception {
		private:
			std::string	_msg;
		public:
			explicit BaseException(const std::string &msg) : _msg(msg) {};
			const char *what() const noexcept override { return this->_msg.c_str(); };
		};

		class InitializationErrorException : public BaseException {
		public:
			explicit InitializationErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class StreamCreationErrorException : public BaseException {
		public:
			explicit StreamCreationErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class OpenStreamErrorException : public BaseException {
		public:
			explicit OpenStreamErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class StartStreamErrorException : public BaseException {
		public:
			explicit StartStreamErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class StreamActivityCheckErrorException : public BaseException {
		public:
			explicit StreamActivityCheckErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class CloseStreamErrorException : public BaseException {
		public:
			explicit CloseStreamErrorException(const std::string &msg) : BaseException(msg) {};
		};

		class InvalidChannelCountException : public BaseException {
		public:
			explicit InvalidChannelCountException(const std::string &msg) : BaseException(msg) {};
		};

		class AlreadyWorkingException : public BaseException {
		public:
			explicit AlreadyWorkingException(const std::string &msg) : BaseException(msg) {};
		};

		class NoOutputDeviceException : public BaseException {
		public:
			explicit NoOutputDeviceException(const std::string &msg) : BaseException(msg) {};
		};

		class NoInputDeviceException : public BaseException {
		public:
			explicit NoInputDeviceException(const std::string &msg) : BaseException(msg) {};
		};
	}

	class Sound {
	public:
		struct SoundState;
		typedef std::function<int (const float *, float *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, SoundState &)> PortAudioHandler;

		struct SoundState {
			bool stopped;
			unsigned int channelCount;
			unsigned int currentIndex;
			std::vector<float> buffer;
			PortAudioHandler handler;
			Network::Socket *socket;
		};

		Sound();
		~Sound();
		std::vector<float> recordAudio(float duration, size_t sampleRate = 48000, unsigned channelCount = 2);
		void playBuffer(const std::vector<float> &buffer, size_t sampleRate = 48000, unsigned channelCount = 2);
		void recordAudioToSocket(Network::Socket &, size_t sampleRate = 48000, unsigned channelCount = 2);
		void playFromSocket(Network::Socket &, size_t sampleRate = 48000, unsigned channelCount = 2);
		void stopActions();
		bool isReady() const;

	private:

		SoundState _state;
		std::thread _mainThread;

		static int _defaultCallback(
			const void *inputBuffer,
			void *outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *timeInfo,
			PaStreamCallbackFlags statusFlags,
			void *userData
		);

		static PortAudioHandler _playBufferCallback;
		static PortAudioHandler _recordBufferCallback;
		static PortAudioHandler _playSocketCallback;
		static PortAudioHandler _recordSocketCallback;

		void _play(const PortAudioHandler &, size_t sampleRate, unsigned channelCount);
		void _record(const PortAudioHandler &, size_t sampleRate, unsigned channelCount);
		void _setupStream(size_t sampleRate, unsigned channelCount, bool input, bool output);
	};
}

#endif //BABEL_SOUND_HPP
