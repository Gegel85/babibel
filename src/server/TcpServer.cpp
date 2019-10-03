/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.cpp
*/

#include <iostream>
#include "TcpServer.hpp"
#include "../network/Protocol.hpp"
#include "../network/SocketExceptions.hpp"

namespace Babel::Server
{
	void TcpServer::sendPacket(Network::Socket &socket, Network::Protocol::Opcode op, const std::string &data)
	{
		Network::Protocol::Packet packet;

		packet.op = op;
		packet.data = data;
		socket.send(packet);
	}

	void TcpServer::disconnectClient(Network::Socket &socket, const std::string &code)
	{
		TcpServer::sendPacket(socket, Network::Protocol::BYE, code);
		socket.disconnect();
		std::cout << "Client has been disconnected" << std::endl;
	}

	TcpServer::TcpServer(unsigned short port) :
		_socket(port),
		_handler{[this](Network::Socket &socket){
			try {
				Network::Protocol::Packet packet{socket};

				std::cout << "Received new packet (" << packet << ")" << std::endl;

				switch (packet.op) {
				case Network::Protocol::HELLO:
					if (packet.data.size() != 2)
						return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_PACKET);
					std::cout << "Client version is ";
					std::cout << static_cast<unsigned>(static_cast<unsigned char>(packet.data[0] << 8) + static_cast<unsigned char>(packet.data[1]));
					std::cout << std::endl;
					if (packet.data != VERSION_STR)
						return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_VERSION);
					break;
				case Network::Protocol::BYE:
					return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::NORMAL_CLOSURE);
				case Network::Protocol::OK:
				case Network::Protocol::KO:
					return TcpServer::sendPacket(socket, Network::Protocol::OK, "");
				case Network::Protocol::LOGIN:
				case Network::Protocol::REGISTER:
					if (packet.data.size() != 32)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET);
					if (this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::ALREADY_CONNECTED);
					this->_users.at(&socket).connected = true;
					this->_users.at(&socket).userId = this->_lastUserID;
					this->_createdUsers.push_back({
						this->_lastUserID,
						packet.data.substr(0, 16),
						packet.data.substr(16, 16)
					});
					return TcpServer::sendPacket(
						socket,
						Network::Protocol::OK,
						Network::Protocol::Packet::toByteString(this->_lastUserID++) + packet.data.substr(0, 16)
					);
				case Network::Protocol::LOGOUT:
					if (!this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED);
					this->_users.at(&socket).connected = false;
					this->_users.at(&socket).userId = 0;
					return TcpServer::sendPacket(socket, Network::Protocol::OK, "");
				case Network::Protocol::GET_FRIENDS:
				case Network::Protocol::GET_USER_INFOS:
				case Network::Protocol::CALL:
				case Network::Protocol::CALL_ACCEPTED:
				case Network::Protocol::CALL_REFUSED:
					return;
				default:
					return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_OPCODE);
				}
			} catch (Network::Exceptions::TimeoutException &) {
				TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_PACKET);
			}
		}}
	{}

	void TcpServer::run()
	{
		Network::Protocol::Packet packet;

		packet.op = Network::Protocol::HELLO;
		packet.data = VERSION_STR;
		std::cout << "Wating for connection..." << std::endl;
		while (true) {
			try {
				this->_socket.waitToBeReady(1);
				for (bool done = false; !done; ) {
					done = true;
					for (auto it = this->_users.begin(); it != this->_users.end(); )
						if (!it->first->isOpen()) {
							this->_users.erase(it);
							done = false;
							break;
						} else
							it++;
				}

				Network::Socket &sock = this->_socket.acceptClient(this->_handler);

				printf("New client connected\n");
				sock.send(packet);
				this->_users.emplace(&sock, Client{0, false, 0, 0});
			} catch (Network::Exceptions::TimeoutException &) {
			} catch (Network::Exceptions::EOFException &) {}
		}
	}
}