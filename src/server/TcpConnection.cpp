/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpConnection.cpp
*/

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "TcpConnection.hpp"

namespace Babel
{
	void TcpConnection::start()
	{
		this->_packet.op = Protocol::HELLO;
		this->_packet.data = {'\0', '\x01'};
		this->_buffer = this->_packet;

		std::cout << "New connection from " << this->_socket.remote_endpoint() << std::endl;
		boost::asio::async_write(
			this->_socket,
			boost::asio::buffer(this->_buffer),
			boost::bind(
				&TcpConnection::_handleWrite,
				this->shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	TcpConnection::Ptr TcpConnection::create(boost::asio::io_context &io_context)
	{
		return Ptr(new TcpConnection(io_context));
	}

	boost::asio::ip::tcp::socket& TcpConnection::socket()
	{
		return this->_socket;
	}

	TcpConnection::TcpConnection(boost::asio::io_context &io_context) :
		_socket(io_context)
	{
	}

	TcpConnection::~TcpConnection()
	{
		this->_packet.op = Protocol::BYE;
		this->_packet.data = Protocol::ExitReason::NORMAL_CLOSURE;
		this->_buffer = this->_packet;

		this->_socket.send(boost::asio::buffer(this->_buffer));
		std::cout << this->_socket.remote_endpoint() << " disconnected" << std::endl;
	}

	void TcpConnection::_handleWrite(const boost::system::error_code &, size_t)
	{}
}