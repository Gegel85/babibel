/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.cpp
*/

#include "TcpServer.hpp"
#include "../network/Protocol.hpp"
#include "../network/SocketExceptions.hpp"

namespace Babel
{
	void TcpServer::sendPacket(Babel::Socket &socket, Babel::Protocol::Opcode op, const std::string &data)
	{
		Protocol::Packet packet;

		packet.op = op;
		packet.data = data;
		socket.send(packet);
	}

	void TcpServer::disconnectClient(Babel::Socket &socket, const std::string &code)
	{
		TcpServer::sendPacket(socket, Protocol::BYE, code);
		socket.disconnect();
	}

	TcpServer::TcpServer(unsigned short port) :
		_socket(port),
		_handler{[this](Socket &socket){
			try {
				Protocol::Packet packet{socket};

				switch (packet.op) {
					case Protocol::HELLO:
						if (packet.data != VERSION_STR)
							TcpServer::disconnectClient(socket, Protocol::ErrorReason::BAD_VERSION);
						break;
					case Protocol::BYE:
						TcpServer::disconnectClient(socket, Protocol::ErrorReason::NORMAL_CLOSURE);
						break;
					case Protocol::OK:
					case Protocol::KO:
						TcpServer::sendPacket(socket, Protocol::OK, "");
						break;
					case Protocol::CONNECT:
					case Protocol::REGISTER:
					case Protocol::GET_FRIENDS:
					case Protocol::GET_USER_INFOS:
					case Protocol::CALL:
					case Protocol::CALL_ACCEPTED:
					case Protocol::CALL_REFUSED:
						break;
					default:
						TcpServer::disconnectClient(socket, Protocol::ErrorReason::BAD_OPCODE);
				}
			} catch (TimeoutException &) {
				TcpServer::disconnectClient(socket, Protocol::ErrorReason::BAD_PACKET);
			}
		}}
	{}

	void TcpServer::run()
	{
		Protocol::Packet packet;

		packet.op = Protocol::HELLO;
		packet.data = VERSION_STR;
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

				Socket &sock = this->_socket.acceptClient(this->_handler);

				printf("New client connected\n");
				sock.send(packet);
				this->_users.emplace(&sock, Client{0, false, 0, 0});
			} catch (TimeoutException &) {}
		}
	}
}