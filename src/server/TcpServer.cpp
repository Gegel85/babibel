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

namespace BabelServer
{
	void TcpServer::sendPacket(BabelNetwork::Socket &socket, BabelNetwork::Protocol::Opcode op, const std::string &data)
	{
		BabelNetwork::Protocol::Packet packet;

		packet.op = op;
		packet.data = data;
		socket.send(packet);
	}

	void TcpServer::disconnectClient(BabelNetwork::Socket &socket, const std::string &code)
	{
		TcpServer::sendPacket(socket, BabelNetwork::Protocol::BYE, code);
		socket.disconnect();
		std::cout << "Client has been disconnected" << std::endl;
	}

	TcpServer::TcpServer(unsigned short port) :
		_socket(port),
		_handler{[this](BabelNetwork::Socket &socket){
			try {
				BabelNetwork::Protocol::Packet packet{socket};

				std::cout << "Received new packet (" << packet << ")" << std::endl;

				switch (packet.op) {
				case BabelNetwork::Protocol::HELLO:
					if (packet.data.size() != 2)
						return TcpServer::disconnectClient(socket, BabelNetwork::Protocol::ErrorReason::BAD_PACKET);
					std::cout << "Client version is ";
					std::cout << static_cast<unsigned>(static_cast<unsigned char>(packet.data[0] << 8) + static_cast<unsigned char>(packet.data[1]));
					std::cout << std::endl;
					if (packet.data != VERSION_STR)
						return TcpServer::disconnectClient(socket, BabelNetwork::Protocol::ErrorReason::BAD_VERSION);
					break;
				case BabelNetwork::Protocol::BYE:
					return TcpServer::disconnectClient(socket, BabelNetwork::Protocol::ErrorReason::NORMAL_CLOSURE);
				case BabelNetwork::Protocol::OK:
				case BabelNetwork::Protocol::KO:
					return TcpServer::sendPacket(socket, BabelNetwork::Protocol::OK, "");
				case BabelNetwork::Protocol::LOGIN:
				case BabelNetwork::Protocol::REGISTER:
					if (packet.data.size() != 32)
						return TcpServer::sendPacket(socket, BabelNetwork::Protocol::KO, BabelNetwork::Protocol::ErrorReason::BAD_PACKET);
					if (this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, BabelNetwork::Protocol::KO, BabelNetwork::Protocol::ErrorReason::ALREADY_CONNECTED);
					this->_users.at(&socket).connected = true;
					this->_users.at(&socket).userId = this->_lastUserID;
					this->_createdUsers.push_back({
						this->_lastUserID,
						packet.data.substr(0, 16),
						packet.data.substr(16, 16)
					});
					return TcpServer::sendPacket(
						socket,
                        BabelNetwork::Protocol::OK,
                        BabelNetwork::Protocol::Packet::toByteString(this->_lastUserID++) + packet.data.substr(0, 16)
					);
				case BabelNetwork::Protocol::LOGOUT:
					if (!this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, BabelNetwork::Protocol::KO, BabelNetwork::Protocol::ErrorReason::NOT_CONNECTED);
					this->_users.at(&socket).connected = false;
					this->_users.at(&socket).userId = 0;
					return TcpServer::sendPacket(socket, BabelNetwork::Protocol::OK, "");
				case BabelNetwork::Protocol::GET_FRIENDS:
				case BabelNetwork::Protocol::GET_USER_INFOS:
				case BabelNetwork::Protocol::CALL:
				case BabelNetwork::Protocol::CALL_ACCEPTED:
				case BabelNetwork::Protocol::CALL_REFUSED:
					return;
				default:
					return TcpServer::disconnectClient(socket, BabelNetwork::Protocol::ErrorReason::BAD_OPCODE);
				}
			} catch (BabelNetwork::TimeoutException &) {
				TcpServer::disconnectClient(socket, BabelNetwork::Protocol::ErrorReason::BAD_PACKET);
			}
		}}
	{}

	void TcpServer::run()
	{
        BabelNetwork::Protocol::Packet packet;

		packet.op = BabelNetwork::Protocol::HELLO;
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

                BabelNetwork::Socket &sock = this->_socket.acceptClient(this->_handler);

				printf("New client connected\n");
				sock.send(packet);
				this->_users.emplace(&sock, Client{0, false, 0, 0});
			} catch (BabelNetwork::TimeoutException &) {
			} catch (BabelNetwork::EOFException &) {}
		}
	}
}