//
// Created by Gegel85 on 05/04/2019.
//

#include <cstring>
#include <sstream>
#include <iostream>
#include "Socket.hpp"
#include "SocketExceptions.hpp"

#ifndef _WIN32
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <sys/select.h>
#	include <unistd.h>
#define closesocket(socket) close(socket)

typedef fd_set FD_SET;
#else
#	include <windows.h>
#endif

namespace Babel::Network
{
	std::string Socket::getLastSocketError()
	{
#ifdef _WIN32
		char *s = nullptr;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&s,
			0,
			nullptr
		);

		return s;
#else
		return strerror(errno);
#endif
	}

	Socket::Socket()
	{
	#ifdef _WIN32
		WSADATA			WSAData;
		WSAStartup(MAKEWORD(2,0), &WSAData);
	#endif
	}

	Socket::Socket(SOCKET sock, unsigned remoteIp) :
		_socket(sock),
		_remoteIp(remoteIp)
	{
	}

	Socket::~Socket()
	{
		if (this->_socket != INVALID_SOCKET)
			closesocket(this->_socket);
	}

	void Socket::connect(const std::string &host, unsigned short portno, int type, int protocol)
	{
		struct hostent	*server;

		if (this->isOpen())
			throw Exceptions::AlreadyOpenedException("This socket is already opened");

		/* lookup the ip address */
		server = gethostbyname(host.c_str());
		if (server == nullptr)
			throw Exceptions::HostNotFoundException("Cannot find host '" + host + "'");

		this->connect(*reinterpret_cast<unsigned *>(server->h_addr), portno, type, protocol);
	}

	void Socket::connect(unsigned int ip, unsigned short portno, int type, int protocol)
	{
		struct sockaddr_in	serv_addr = {};

		if (this->isOpen())
			throw Exceptions::AlreadyOpenedException("This socket is already opened");

		/* fill in the structure */
		std::memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(portno);
		serv_addr.sin_addr.s_addr = ip;

		/* create the socket */
		this->_socket = socket(AF_INET, type, protocol);
		if (this->_socket == INVALID_SOCKET)
			throw Exceptions::SocketCreationErrorException(getLastSocketError());

		/* connect the socket */
		if (::connect(this->_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			closesocket(this->_socket);
			this->_socket = INVALID_SOCKET;
			throw Exceptions::ConnectException(std::string("Cannot connect to ") + inet_ntoa(serv_addr.sin_addr));
		}
		this->_type = type;
		this->_port = portno;
		this->_remoteIp = ip;
		this->_protocol = protocol;
	}

	void Socket::disconnect()
	{
		if (!this->isOpen())
			return;
		closesocket(this->_socket);
		this->_socket = INVALID_SOCKET;
	}

	SOCKET Socket::getSocket() const
	{
		return this->_socket;
	}

	void Socket::setSocket(SOCKET sock, unsigned remoteIp)
	{
		if (this->isOpen())
			throw Exceptions::AlreadyOpenedException("This socket is already opened");

		this->_remoteIp = remoteIp;
		this->_socket = sock;
	}

	std::string Socket::read(int size, int timeout)
	{
		std::string	buf;
		char		buffer[1024];
		sockaddr_in	addr{};
		socklen_t	len = sizeof(addr);

		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = this->_remoteIp;
		addr.sin_port = htons(this->_port);
		addr.sin_family = AF_INET;

		while (size != 0) {
			this->waitToBeReady(timeout);

			int bytes = recvfrom(this->_socket, buffer, (static_cast<unsigned>(size) >= sizeof(buffer)) ? (sizeof(buffer)) : (size), 0, reinterpret_cast<sockaddr *>(&addr), &len);

			if (bytes <= 0) {
				if (size < 0)
					break;
				throw Exceptions::EOFException(bytes ? getLastSocketError() : "The connection was closed");
			}
			for (int i = 0; i < bytes; i++)
				buf.push_back(buffer[i]);
			size -= bytes;
		}
		return buf;
	}

	void Socket::send(const std::string &msg)
	{
		unsigned	pos = 0;
		sockaddr_in	addr{};

		std::memset(&addr, 0, sizeof(addr));
		addr.sin_port = htons(this->_port);
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = this->_remoteIp;
		while (pos < msg.length()) {
			int bytes = ::sendto(this->_socket, &msg.c_str()[pos], msg.length() - pos, 0, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

			if (bytes <= 0)
				throw Exceptions::EOFException(bytes ? getLastSocketError() : "The connection was closed");
			pos += bytes;
		}
	}

	std::string Socket::readUntilEOF(int timeout)
	{
		return this->read(-1, timeout);
	}

	void Socket::setRemoteIp(unsigned ip)
	{
		this->_remoteIp = ip;
	}

	void Socket::waitToBeReady(int timeout) const
	{
		FD_SET	set;
		timeval time = {timeout, 0};

		if (!this->isOpen())
			throw Exceptions::NotConnectedException("This socket is not opened");

		FD_ZERO(&set);
		FD_SET(this->_socket, &set);

		int found = select(FD_SETSIZE, &set, nullptr, nullptr, (timeout > 0 ? &time : nullptr));

		if (found == 0)
			throw Exceptions::TimeoutException("Connection timed out after " + std::to_string(timeout) + " second(s)");
	}

	bool	Socket::isOpen() const
	{
		FD_SET	set;
		timeval time = {0, 0};

		FD_ZERO(&set);
		FD_SET(this->_socket, &set);
		if (this->_socket != INVALID_SOCKET && select(FD_SETSIZE, &set, nullptr, nullptr, &time) == -1) {
			closesocket(this->_socket);
			this->_socket = INVALID_SOCKET;
		}
		return this->_socket != INVALID_SOCKET;
	}

	unsigned Socket::getRemoteIp() const
	{
		if (!this->isOpen())
			throw Exceptions::NotConnectedException("This socket is not opened");
		return this->_remoteIp;
	}
}
