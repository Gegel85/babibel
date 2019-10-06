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
	ServerSocket::~ServerSocket()
	{
		for (auto &thread : this->_threads)
			if (thread.joinable())
				thread.join();
	}

	void ServerSocket::bind(unsigned short port, int type, int protocol)
	{
		struct sockaddr_in address{};

		this->setSocket(socket(AF_INET, type, protocol));
		this->_port = port;
		address.sin_port = htons(port);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		if (::bind(this->_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
			throw Exceptions::BindFailedException(getLastSocketError());
		if (listen(this->_socket, MAX_SOCKETS) == -1)
			throw Exceptions::ListenFailedException(getLastSocketError());
	}

	Socket& ServerSocket::acceptClient(const std::function<void(Socket &)> &handler)
	{
		if (!this->isOpen())
			throw Exceptions::NotConnectedException("The server socket is not bound to a port. Call bind first.");

		if (!handler)
			throw Exceptions::InvalidHandlerException("The data handler sent to acceptClient is empty");

		unsigned i = 0;

		while (i < this->_clients.size() && this->_clients.at(i)->isOpen())
			i++;

		if (i == this->_clients.size())
			this->_clients.emplace_back(new Socket());

		Socket &socket = *this->_clients.at(i);
		sockaddr_in address{};
		socklen_t size = sizeof(address);
		SOCKET sock = ::accept(
			this->getSocket(),
			reinterpret_cast<struct sockaddr *>(&address),
			&size
		);

		socket.setSocket(
			sock,
			address.sin_addr.s_addr
		);

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

	void ServerSocket::disconnectClients()
	{
		this->_clients.clear();
	}
}