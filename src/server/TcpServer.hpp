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
			unsigned	userId;
			bool		connected;
			unsigned	callingUser;
			unsigned	beingCalled;
		};

		struct User {
			unsigned id;
			std::string name;
			std::string passwd;
		};

		std::vector<User> _createdUsers;
		unsigned _lastUserID = 0;
		Babel::Network::ServerSocket _socket;
		std::function<void (Babel::Network::Socket &)> _handler;
		std::map<Babel::Network::Socket *, Client> _users;

	public:
		static void sendPacket(Babel::Network::Socket &, Babel::Network::Protocol::Opcode op, const std::string &data);
		static void disconnectClient(Babel::Network::Socket &, const std::string &code);

		TcpServer(unsigned short port);
		void run();
	};
}


#endif //CPP_BABEL_2019_TCPSERVER_HPP
