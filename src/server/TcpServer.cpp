/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TcpServer.cpp
*/

#include <iostream>
#include "TcpServer.hpp"
#include "../network/Protocol.hpp"
#include "../network/SocketExceptions.hpp"
#include <algorithm>

namespace Babel::Server
{
	void TcpServer::sendPacket(Network::Socket &socket, Network::Protocol::Opcode op, const std::string &data)
	{
		Network::Protocol::Packet packet;

		packet.op = op;
		packet.data = data;
		socket.send(packet);
	}

	void TcpServer::disconnectClient(Network::Socket &socket, const std::string &code)
	{
		TcpServer::sendPacket(socket, Network::Protocol::BYE, code);
		socket.disconnect();
		std::cout << "Client has been disconnected" << std::endl;
	}

	TcpServer::TcpServer(unsigned short port) :
		_handler{[this](Network::Socket &socket){
			try {
				Network::Protocol::Packet packet{socket};

				std::cout << "Received new packet (" << packet << ")" << std::endl;

				switch (packet.op) {
				case Network::Protocol::HELLO:
					if (packet.data.size() != 2)
						return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_PACKET);
					std::cout << "Client version is ";
					std::cout << static_cast<unsigned>(static_cast<unsigned char>(packet.data[0] << 8) + static_cast<unsigned char>(packet.data[1]));
					std::cout << std::endl;
					if (packet.data != VERSION_STR)
						return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_VERSION);
					break;
				case Network::Protocol::BYE:
					return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::NORMAL_CLOSURE);
				case Network::Protocol::OK:
				case Network::Protocol::KO:
					return TcpServer::sendPacket(socket, Network::Protocol::OK, "");
				case Network::Protocol::LOGIN:
				case Network::Protocol::REGISTER:
					if (packet.data.size() != 64)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET);
					if (this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::ALREADY_CONNECTED);
					this->_users.at(&socket).connected = true;
					this->_users.at(&socket).userId = this->_lastUserID;
					this->_createdUsers.push_back({
						this->_lastUserID,
						packet.data.substr(0, 32),
						packet.data.substr(32, 32)
					});
					return TcpServer::sendPacket(
						socket,
						Network::Protocol::OK,
						Network::Protocol::Packet::uint32toByteString(this->_lastUserID++) + packet.data.substr(0, 16)
					);
				case Network::Protocol::LOGOUT:
					if (!this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED);
					this->_users.at(&socket).connected = false;
					this->_users.at(&socket).userId = 0;
					this->_users.at(&socket).beingCalled = 0;
					this->_users.at(&socket).callingUser = 0;
					return TcpServer::sendPacket(socket, Network::Protocol::OK, "");
				case Network::Protocol::GET_FRIENDS:
					return this->_getFriends(socket);
				case Network::Protocol::CALL:
					if (!this->_users.at(&socket).connected)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED);
					if (packet.data.size() != 4)
						return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET);
					return this->_callUser(socket, Network::Protocol::Packet::uint32FromByteString(packet.data));
				case Network::Protocol::GET_USER_INFOS:
					return;
				case Network::Protocol::CALL_ACCEPTED:
					if (!packet.data.empty())
						return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET));
					return this->_acceptCall(socket);
				case Network::Protocol::CALL_REFUSED:
					if (!packet.data.empty())
						return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET));
					return this->_refuseCall(socket);
				case Network::Protocol::ADD_FRIEND:
					if (packet.data.size() != 4)
						return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET));
					return (this->_addFriend(socket, Network::Protocol::Packet::uint32FromByteString( packet.data)));
				case Network::Protocol::REMOVE_FRIEND:
					if (packet.data.size() != 4)
						return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::BAD_PACKET));
					return (this->_removeFriend(socket, Network::Protocol::Packet::uint32FromByteString(packet.data)));
				default:
					return TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_OPCODE);
				}
			} catch (Network::Exceptions::TimeoutException &) {
				TcpServer::disconnectClient(socket, Network::Protocol::ErrorReason::BAD_PACKET);
			}
		}}
	{
		this->_socket.bind(port);
	}

	void TcpServer::run()
	{
		Network::Protocol::Packet packet;

		packet.op = Network::Protocol::HELLO;
		packet.data = VERSION_STR;
		std::cout << "Waiting for connection..." << std::endl;
		while (true) {
			try {
				this->_socket.waitToBeReady(1);
				for (bool done = false; !done; ) {
					done = true;
					for (auto it = this->_users.begin(); it != this->_users.end(); )
						if (!it->first->isOpen()) {
							this->_users.erase(it);
							done = false;
							break;
						} else
							it++;
				}

				Network::Socket &sock = this->_socket.acceptClient(this->_handler);

				printf("New client connected\n");
				sock.send(packet);
				this->_users.emplace(&sock, Client{0, false, 0, 0, {}});
			} catch (Network::Exceptions::TimeoutException &) {
			} catch (Network::Exceptions::EOFException &) {}
		}
	}

	void TcpServer::_getFriends(Network::Socket &socket)
	{
		auto &friendList = this->_users.at(&socket).friendList;
		std::string data;

		if (!this->_users.at(&socket).connected)
			return(TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED));
		data.reserve(friendList.size() * 4);
		for (auto value : friendList)
			data += Network::Protocol::Packet::uint32toByteString(value);
		TcpServer::sendPacket(socket, Network::Protocol::OK, data);
	}

	void TcpServer::_addFriend(Network::Socket &socket, unsigned int ID)
	{
		auto &user = this->_users.at(&socket);

		if (!user.connected)
			return(TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED));

		auto finder = std::find(user.friendList.begin(), user.friendList.end(), ID);
		if (finder == user.friendList.end()) {
			user.friendList.push_back(ID);
			TcpServer::sendPacket(socket, Network::Protocol::OK, "");
		}
		else
			return(TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_FOUND));
	}

	void TcpServer::_removeFriend(Network::Socket &socket, unsigned int ID) {
		auto &user = this->_users.at(&socket);

		if (!this->_users.at(&socket).connected)
			return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_CONNECTED));
		auto finder = std::find(user.friendList.begin(), user.friendList.end(), ID);
		if (finder != user.friendList.end()) {
			user.friendList.erase(finder);
			TcpServer::sendPacket(socket, Network::Protocol::OK, "");
		} else
			return (TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_FOUND));
	}

	void TcpServer::_callUser(Babel::Network::Socket &socket, unsigned id)
	{
		try {
			auto user = this->_getUser(id);
			auto &me = this->_users.at(&socket);
			auto &remote = this->_users.at(user.first);

			if (me.userId == remote.userId)
				return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_AUTHORIZED);

			if (me.callingUser)
				return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::ALREADY_CALLING);

			if (remote.beingCalled)
				return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::ALREADY_USED);

			me.callingUser = id;
			remote.beingCalled = me.userId;
			this->sendPacket(*user.first, Network::Protocol::CALL, Network::Protocol::Packet::uint32toByteString(me.userId));
		} catch (Exceptions::UserNotFoundException &) {
			return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::REMOTE_NOT_CONNECTED);
		}
	}

	void TcpServer::_acceptCall(Babel::Network::Socket &socket)
	{
		try {
			auto &me = this->_users.at(&socket);

			if (!me.beingCalled)
				return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_FOUND);

			auto user = this->_getUser(me.beingCalled);
			auto &remote = this->_users.at(user.first);

			me.callingUser = 0;
			remote.beingCalled = 0;
			this->sendPacket(
				socket,
				Network::Protocol::CALL_ACCEPTED,
				Network::Protocol::Packet::uint32toByteString(user.first->getRemoteIp()) +
				Network::Protocol::Packet::uint16toByteString(DEFAULT_PORT)
			);
			this->sendPacket(
				*user.first,
				Network::Protocol::CALL_ACCEPTED,
				Network::Protocol::Packet::uint32toByteString(0) +
				Network::Protocol::Packet::uint16toByteString(DEFAULT_PORT)
			);
		} catch (Exceptions::UserNotFoundException &) {
			return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::REMOTE_NOT_CONNECTED);
		}
	}

	void TcpServer::_refuseCall(Babel::Network::Socket &socket)
	{
		try {
			auto &me = this->_users.at(&socket);

			if (!me.beingCalled)
				return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::NOT_FOUND);

			auto user = this->_getUser(me.beingCalled);
			auto &remote = this->_users.at(user.first);

			me.callingUser = 0;
			remote.beingCalled = 0;
			this->sendPacket(
				socket,
				Network::Protocol::OK,
				""
			);
			this->sendPacket(
				*user.first,
				Network::Protocol::CALL_REFUSED,
				""
			);
		} catch (Exceptions::UserNotFoundException &) {
			return TcpServer::sendPacket(socket, Network::Protocol::KO, Network::Protocol::ErrorReason::REMOTE_NOT_CONNECTED);
		}
	}

	std::pair<Network::Socket *, std::reference_wrapper<TcpServer::User>> TcpServer::_getUser(unsigned id)
	{
		for (auto &user : this->_users)
			if (user.second.connected && user.second.userId == id)
				return {user.first, this->_createdUsers[user.second.userId]};
		throw Exceptions::UserNotFoundException("Cannot find user with id " + std::to_string(id));
	}
}
