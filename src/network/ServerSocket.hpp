/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ServerSocket.hpp
*/

#ifndef CPP_BABEL_2019_SERVERSOCKET_HPP
#define CPP_BABEL_2019_SERVERSOCKET_HPP


#include <functional>
#include <vector>
#include <thread>
#include "Socket.hpp"

#define MAX_SOCKETS 32

namespace Babel::Network
{
	//! @brief Socket du serveur où se connecte les clients.
	class ServerSocket : public Socket {
	private:
		bool _ended;
		std::vector<std::thread> _threads;
		std::vector<std::unique_ptr<Socket>> _clients;

	public:
		~ServerSocket();
		void bind(unsigned short port, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
		Socket &acceptClient(const std::function<void (Socket &)> &handler = {});
		void disconnectClients();
	};
}


#endif //CPP_BABEL_2019_SERVERSOCKET_HPP
