/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpConnection.hpp
*/

#ifndef CPP_BABEL_2019_TCPCONNECTION_HPP
#define CPP_BABEL_2019_TCPCONNECTION_HPP


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include "../Protocol.hpp"

namespace Babel
{
	class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
	{
	public:
		typedef boost::shared_ptr<TcpConnection> Ptr;

		~TcpConnection();
		static Ptr create(boost::asio::io_context& io_context);
		boost::asio::ip::tcp::socket &socket();
		void start();

	private:
		TcpConnection(boost::asio::io_context &io_context);

		void _handleWrite(const boost::system::error_code &error, size_t bytesTransferred);

		boost::asio::ip::tcp::socket _socket;
		Protocol::Packet _packet;
		std::string _buffer;
	};
}


#endif //CPP_BABEL_2019_TCPCONNECTION_HPP
