/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ServerSocket.cpp
*/

#include <iostream>
#include "ServerSocket.hpp"
#include "SocketExceptions.hpp"

namespace Babel::Network
{
	ServerSocket::ServerSocket(unsigned short port) :
		Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
		_port(port)
	{
		struct sockaddr_in address{};

		address.sin_port = htons(this->_port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		if (bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
			throw Exceptions::BindFailedException(getLastSocketError());
		if (listen(this->_socket, MAX_SOCKETS) == -1)
			throw Exceptions::ListenFailedException(getLastSocketError());
	}

	ServerSocket::~ServerSocket()
	{
		for (auto &thread : this->_threads)
			if (thread.joinable())
				thread.join();
	}

	Socket& ServerSocket::acceptClient(const std::function<void(Socket &)> &handler)
	{
		if (!handler)
			throw Exceptions::InvalidHandlerException("The data handler sent to acceptClient is empty");

		int i = 0;

		while (i < this->_clients.size() && this->_clients.at(i)->isOpen())
			i++;

		if (i == this->_clients.size())
			this->_clients.emplace_back(new Socket());

		Socket &socket = *this->_clients.at(i);

		socket.setSocket(::accept(this->getSocket(), nullptr, nullptr));
		this->_threads.emplace_back([&socket, handler, this](){
			Socket &sock = socket;

			while (this->isOpen() && sock.isOpen())
				try {
					sock.waitToBeReady(1);
					handler(sock);
				} catch (Exceptions::TimeoutException &) {
				} catch (std::exception &e) {
					std::cerr << "Error while receiving data " << e.what() << std::endl;
					try { sock.disconnect(); } catch (std::exception &) {}
				}
		});
		return socket;
	}
}