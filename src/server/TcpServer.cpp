/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.cpp
*/

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "TcpServer.hpp"
#include "TcpConnection.hpp"

namespace Babel
{
	TcpServer::TcpServer(unsigned int port) :
		_context(),
		_acceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
		startAccept();
	}

	void TcpServer::handleAccept(TcpConnection::Ptr newConnection, const boost::system::error_code &error)
	{
		if (!error) {
			newConnection->start();
		}

		startAccept();
	}

	void TcpServer::startAccept()
	{
		TcpConnection::Ptr new_connection = TcpConnection::create(this->_context);

		this->_acceptor.async_accept(
			new_connection->socket(),
			boost::bind(
				&TcpServer::handleAccept,
				this,
				new_connection,
				boost::asio::placeholders::error
			)
		);
	}

	void TcpServer::run()
	{
		this->_context.run();
	}
}