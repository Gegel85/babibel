/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.hpp
*/

#ifndef CPP_BABEL_2019_TCPSERVER_HPP
#define CPP_BABEL_2019_TCPSERVER_HPP


#include <map>
#include "../network/ServerSocket.hpp"
#include "../network/Protocol.hpp"

namespace Babel::Server
{
	class TcpServer {
	private:
		struct Client {
			bool     connected;
			unsigned userId;
			unsigned callingUser;
			unsigned beingCalled;
			std::vector<unsigned int> friendList;
		};

		struct User {
			unsigned    id;
			std::string name;
			std::string passwd;
		};

		std::vector<User> _createdUsers;
		unsigned _lastUserID = 0;
		Network::ServerSocket _socket;
		std::function<void (Network::Socket &)> _handler;
		std::map<Network::Socket *, Client> _users;

		void _getFriends(Network::Socket &socket);
		void _callUser(Network::Socket &socket, unsigned id);
	public:
		static void sendPacket(Network::Socket &, Network::Protocol::Opcode op, const std::string &data);
		static void disconnectClient(Network::Socket &, const std::string &code);

		TcpServer(unsigned short port);
		void run();
	};
}


#endif //CPP_BABEL_2019_TCPSERVER_HPP
