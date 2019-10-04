/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpClient.hpp
*/

#ifndef CPP_BABEL_2019_TCPCLIENT_HPP
#define CPP_BABEL_2019_TCPCLIENT_HPP


#include "../network/Socket.hpp"
#include "../network/ServerSocket.hpp"
#include "../network/Protocol.hpp"

namespace Babel::Client
{
	class TcpClient {
	private:
		bool _voiceConnectionEnd = false;
		bool _serverConnectionEnd = false;
		std::thread _serverThread;
		std::thread _voiceThread;
		std::thread _serverConnectionThread;
		std::thread _voiceConnectionThread;
		std::string _lastError = "";
		std::string _lastOKResponse = "";
		Network::Socket _sock;
		Network::ServerSocket _voiceSock;

		void _handleServerPacket(Network::Protocol::Packet &packet);

		static void _disconnect(bool &end, std::thread &connectThread, std::thread &retryThread, Network::Socket &sock);

	public:
		~TcpClient();
		void connectToServer(const std::string &ip, unsigned short port, unsigned retryTime = 0);
		void connectToVoice(const std::string &ip, unsigned short port, unsigned retryTime = 0);
		void sendPacketToServer(Network::Protocol::Opcode op, const std::string &data);
		void sendPacketToServer(Network::Protocol::Opcode op, unsigned data);
		void hostVoice(unsigned short port, unsigned retryTime = 0);
		void disconnectFromServer(const std::string &reason);
		void disconnectFromServer();
		void disconnectFromVoice();
		void disconnect();

		std::string getLastOKResponse() const;
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
	}
}


#endif //CPP_BABEL_2019_TCPCLIENT_HPP
