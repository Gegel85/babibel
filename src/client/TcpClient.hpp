/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpClient.hpp
*/

#ifndef CPP_BABEL_2019_TCPCLIENT_HPP
#define CPP_BABEL_2019_TCPCLIENT_HPP


#include <optional>
#include "../network/Socket.hpp"
#include "../network/ServerSocket.hpp"
#include "../network/Protocol.hpp"
#include "sound/Sound.hpp"

namespace Babel::Client
{
	class TcpClient {
	private:
		Sound::Sound _recorder;
		Sound::Sound _player;
		bool _serverConnectionEnd = false;
		std::thread _serverThread;
		std::thread _serverConnectionThread;
		std::string _lastError = "";
		std::optional<std::pair<unsigned char, std::string>> _lastResponse = {{Network::Protocol::OK, ""}};
		Network::ServerSocket _voiceSock;
		Network::Socket _sock;

		void _handleServerPacket(Network::Protocol::Packet &packet);

		static void _disconnect(bool &end, std::thread &connectThread, std::thread &retryThread, Network::Socket &sock);

	public:
		~TcpClient();
		void connectToServer(const std::string &ip, unsigned short port, unsigned retryTime = 0);
		void sendPacketToServer(Network::Protocol::Opcode op, const std::string &data);
		std::pair<unsigned char, std::string> waitServerResponse(int timeout = -1);
		void sendPacketToServer(Network::Protocol::Opcode op, unsigned data);
		void connectToVoice(unsigned ip, unsigned short port);
		void disconnectFromServer(const std::string &reason);
		void disconnectFromServer();
		void disconnectFromVoice();
		void disconnect();

		std::string getLastError() const;
		bool isVoiceConnected() const;
		bool isConnected() const;
	};

	namespace Exceptions {
		class BaseException : public std::exception {
		private:
			std::string	_msg;
		public:
			explicit BaseException(const std::string &msg) : _msg(msg) {};
			const char *what() const noexcept override { return this->_msg.c_str(); };
		};

		class DisconnectedException : public BaseException {
		public:
			explicit DisconnectedException(const std::string &msg) : BaseException(msg) {};
		};

		class TimeoutException : public BaseException {
		public:
			explicit TimeoutException(const std::string &msg) : BaseException(msg) {};
		};
	}
}


#endif //CPP_BABEL_2019_TCPCLIENT_HPP
