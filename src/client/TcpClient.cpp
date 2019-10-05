/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpClient.cpp
*/

#include <iostream>
#include "TcpClient.hpp"
#include "../network/SocketExceptions.hpp"
#include "../network/Protocol.hpp"

namespace Babel::Client
{
	TcpClient::~TcpClient()
	{
		this->disconnectFromVoice();
		this->disconnectFromServer();
	}

	void TcpClient::hostVoice(unsigned short port)
	{
		this->disconnectFromVoice();
		std::cout << "Hosting voice server on port " << std::to_string(port) << std::endl;
		try {
			this->_voiceSock.bind(port, SOCK_DGRAM, IPPROTO_UDP);
		} catch (Network::Exceptions::ListenFailedException &) {}
		try {
			this->_voiceSock.waitToBeReady(5);

			this->_player.playFromSocket(this->_voiceSock);
			this->_recorder.playFromSocket(this->_voiceSock);
		} catch (std::exception &e) {
			std::cerr << "Couldn't connect to voice client: " << e.what() << std::endl;
		}
	}

	void TcpClient::connectToVoice(const std::string &ip, unsigned short port)
	{
		this->disconnectFromVoice();
		std::cout << "Connecting to voice server at " << ip << ":" << std::to_string(port) << std::endl;
		try {
			this->_voiceSock.connect(ip, port, SOCK_DGRAM, IPPROTO_UDP);
			this->_recorder.playFromSocket(this->_voiceSock);
			this->_player.playFromSocket(this->_voiceSock);
		} catch (std::exception &e) {
			std::cerr << "Couldn't connect to voice server: " << e.what() << std::endl;
		}
	}

	void TcpClient::connectToServer(const std::string &ip, unsigned short port, unsigned retryTime)
	{
		this->disconnectFromServer();
		this->_serverThread = std::thread{[this, ip, port, retryTime](){
			while (!this->_serverConnectionEnd) {
				try {
					Network::Protocol::Packet packet;

					packet.op = Network::Protocol::HELLO;
					packet.data = VERSION_STR;
					this->_sock.connect(ip, port);
					this->_sock.send(packet);
					this->_serverConnectionThread = std::thread{[this]() {
						Network::Protocol::Packet packet;

						try {
							while (this->_sock.isOpen()) {
								try {
									this->_sock.waitToBeReady(1);
								} catch (Network::Exceptions::TimeoutException &) {
									continue;
								}
								packet = this->_sock;
								std::cout << "Server sent packet " << packet << std::endl;
								this->_handleServerPacket(packet);
							}
						} catch (std::exception &e) {
							std::cerr << "An error occurred and the connection to the server will be interrupted: " << e.what() << std::endl;
							if (this->_sock.isOpen())
								this->_sock.disconnect();
							this->_lastError = e.what();
						}
					}};
					while (this->_sock.isOpen() && !this->_serverConnectionEnd)
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					if (this->_serverConnectionEnd)
						break;
					if (this->_serverConnectionThread.joinable())
						this->_serverConnectionThread.join();
				} catch (std::exception &e) {
					std::cerr << "An error occurred when connecting to server: " << e.what() << std::endl;
					if (this->_sock.isOpen())
						this->_sock.disconnect();
					this->_lastError = e.what();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(retryTime));
			}
		}};
	}

	void TcpClient::_handleServerPacket(Babel::Network::Protocol::Packet &packet)
	{
		this->_lastResponse = std::pair<unsigned char, std::string>{packet.op, packet.data};
		switch (packet.op) {
		case Network::Protocol::HELLO:
			if (packet.data.size() != 2)
				return this->disconnectFromServer(Network::Protocol::ErrorReason::BAD_PACKET);
			std::cout << "Server version is ";
			std::cout << static_cast<unsigned>(static_cast<unsigned char>(packet.data[0] << 8U) + static_cast<unsigned char>(packet.data[1]));
			std::cout << std::endl;
			if (packet.data != VERSION_STR)
				return this->disconnectFromServer(Network::Protocol::ErrorReason::BAD_VERSION);
			break;
		case Network::Protocol::BYE:
			this->disconnectFromServer(Network::Protocol::ErrorReason::NORMAL_CLOSURE);
			throw Exceptions::DisconnectedException("The server closed the connection: " + Network::Protocol::ErrorReason::errorReasonToString(packet.data));
		case Network::Protocol::CALL:
			return this->sendPacketToServer(
				this->isVoiceConnected() ? Network::Protocol::CALL_REFUSED : Network::Protocol::CALL_ACCEPTED,
				""
			);
		case Network::Protocol::CALL_ACCEPTED:
			if (Network::Protocol::Packet::uint32FromByteString(packet.data))
				return this->connectToVoice(
					inet_ntoa({
						Network::Protocol::Packet::uint32FromByteString(packet.data)
					}),
					Network::Protocol::Packet::uint16FromByteString(packet.data.substr(4, 2))
				);
			return this->hostVoice(Network::Protocol::Packet::uint16FromByteString(packet.data.substr(4, 2)));
		case Network::Protocol::CALL_REFUSED:
		case Network::Protocol::OK:
		case Network::Protocol::KO:
			return;
		default:
			return this->disconnectFromServer(Network::Protocol::ErrorReason::BAD_OPCODE);
		}
	}

	void TcpClient::_disconnect(bool &end, std::thread &connectThread, std::thread &retryThread, Babel::Network::Socket &sock)
	{
		if (sock.isOpen())
			sock.disconnect();
		end = true;
		if (retryThread.joinable())
			retryThread.join();
		end = false;
		if (connectThread.joinable())
			connectThread.join();
		std::cout  << "Done !" << std::endl;
	}

	void TcpClient::disconnectFromVoice()
	{
		std::cout << "Disconnecting from voice" << std::endl;
		this->_voiceSock.disconnect();
		this->_recorder.stopActions();
		this->_player.stopActions();
		std::cout << "Done !" << std::endl;
	}

	void TcpClient::disconnectFromServer()
	{
		std::cout << "Disconnecting from server" << std::endl;
		_disconnect(this->_serverConnectionEnd, this->_serverConnectionThread, this->_serverThread, this->_sock);
	}

	void TcpClient::disconnect()
	{
		this->disconnectFromVoice();
		this->disconnectFromServer();
	}

	bool TcpClient::isConnected() const
	{
		return this->_sock.isOpen();
	}

	bool TcpClient::isVoiceConnected() const
	{
		return this->_voiceSock.isOpen();
	}

	std::string TcpClient::getLastError() const
	{
		return this->_lastError;
	}

	std::pair<unsigned char, std::string> TcpClient::waitServerResponse(int timeout)
	{
		this->_lastResponse.reset();
		if (timeout < 0)
			while (!this->_lastResponse)
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		else
			while (!this->_lastResponse) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				if (!this->_lastResponse && timeout - 1 < 0)
					throw Exceptions::TimeoutException("Timed out");
			}
		return this->_lastResponse.value();
	}

	void TcpClient::sendPacketToServer(Network::Protocol::Opcode op, const std::string &data)
	{
		Network::Protocol::Packet packet;

		packet.op = op;
		packet.data = data;
		this->_sock.send(packet);
	}

	void TcpClient::sendPacketToServer(Network::Protocol::Opcode op, unsigned data)
	{
		this->sendPacketToServer(op, Network::Protocol::Packet::uint32toByteString(data));
	}

	void TcpClient::disconnectFromServer(const std::string &reason)
	{
		this->sendPacketToServer(Network::Protocol::BYE, reason);
		this->_sock.disconnect();
	}
}