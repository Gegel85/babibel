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
	std::function<void (Socket &)> TcpServer::_handler{[](Socket &socket){
		try {
			Protocol::Packet packet{socket};

			switch (packet.op) {
			case Protocol::HELLO:
				if (packet.data != VERSION_STR) {
					packet.op = Protocol::BYE;
					packet.data = Protocol::ExitReason::BAD_VERSION;
					socket.send(packet);
					socket.disconnect();
				}
				break;
			case Protocol::BYE:
				packet.op = Protocol::BYE;
				packet.data = Protocol::ExitReason::NORMAL_CLOSURE;
				socket.send(packet);
				socket.disconnect();
				break;
			default:
				packet.op = Protocol::BYE;
				packet.data = Protocol::ExitReason::INVALID_OPCODE;
				socket.send(packet);
				socket.disconnect();
			}
		} catch (TimeoutException &) {
			Protocol::Packet packet;

			packet.op = Protocol::BYE;
			packet.data = Protocol::ExitReason::BAD_PACKET;
			socket.send(packet);
		}
	}};

	TcpServer::TcpServer(unsigned short port) :
		_socket(port)
	{}

	void TcpServer::run()
	{
		Protocol::Packet packet;

		packet.op = Protocol::HELLO;
		packet.data = VERSION_STR;
		while (true) {
			Socket &sock = this->_socket.acceptClient(this->_handler);

			sock.send(packet);
		}
	}
}