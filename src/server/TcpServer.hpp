/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.hpp
*/

#ifndef CPP_BABEL_2019_TCPSERVER_HPP
#define CPP_BABEL_2019_TCPSERVER_HPP


#include <boost/asio/ip/tcp.hpp>
#include "TcpConnection.hpp"

class TcpServer {
private:
	void startAccept();
	void handleAccept(TcpConnection::Ptr newConnection, const boost::system::error_code &error);

	boost::asio::io_context _context;
	boost::asio::ip::tcp::acceptor _acceptor;

public:
	TcpServer(unsigned int port);

	void run();
};


#endif //CPP_BABEL_2019_TCPSERVER_HPP
