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

void TcpConnection::start()
{
	std::string message_ = "test";

	boost::asio::async_write(
		this->_socket,
		boost::asio::buffer(message_),
		boost::bind(
			&TcpConnection::_handleWrite,
			boost::shared_ptr<TcpConnection>{this},
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

void TcpConnection::_handleWrite(const boost::system::error_code &error, size_t bytesTransferred)
{
	std::cout << error << " " << bytesTransferred << std::endl;
}