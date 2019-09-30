/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ServerSocket.hpp
*/

#ifndef CPP_BABEL_2019_SERVERSOCKET_HPP
#define CPP_BABEL_2019_SERVERSOCKET_HPP


#include <functional>
#include <thread>
#include "Socket.hpp"

#define MAX_SOCKETS 32

namespace Babel
{
	class ServerSocket : public Socket {
	private:
		unsigned short				_port;
		bool					_ended;
		std::vector<std::thread>		_threads;
		std::vector<std::unique_ptr<Socket>>	_clients;

	public:
		explicit ServerSocket(unsigned short port);
		Socket &acceptClient(const std::function<void (Socket &)> &handler = {});
	};
}


#endif //CPP_BABEL_2019_SERVERSOCKET_HPP
