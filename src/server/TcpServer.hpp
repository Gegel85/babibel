/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.hpp
*/

#ifndef CPP_BABEL_2019_TCPSERVER_HPP
#define CPP_BABEL_2019_TCPSERVER_HPP


#include "../network/ServerSocket.hpp"

namespace Babel
{
	class TcpServer {
	private:
		ServerSocket _socket;
		static std::function<void (Socket &)> _handler;

	public:
		TcpServer(unsigned short port);
		void run();
	};
}


#endif //CPP_BABEL_2019_TCPSERVER_HPP
