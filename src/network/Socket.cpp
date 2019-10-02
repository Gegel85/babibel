//
// Created by Gegel85 on 05/04/2019.
//

#include <cstring>
#include <sstream>
#include <unistd.h>
#include "Socket.hpp"
#include "SocketExceptions.hpp"

#ifndef _WIN32
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <sys/select.h>
#include <iostream>

typedef fd_set FD_SET;
#endif

#ifndef closesocket
#define closesocket(socket) close(socket)
#endif

namespace Babel
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

	Socket::Socket(SOCKET sock, bool connected)
	{
		this->_socket = sock;
		this->_opened = connected;
	}

	Socket::~Socket()
	{
		if (this->_socket != INVALID_SOCKET)
			closesocket(this->_socket);
	}

	void Socket::connect(const std::string &host, unsigned short portno, int protocol)
	{
		struct hostent	*server;

		if (this->isOpen())
			throw AlreadyOpenedException("This socket is already opened");

		/* lookup the ip address */
		server = gethostbyname(host.c_str());
		if (server == nullptr)
			throw HostNotFoundException("Cannot find host '" + host + "'");

		this->connect(*reinterpret_cast<unsigned *>(server->h_addr), portno, protocol);
	}

	void Socket::connect(unsigned int ip, unsigned short portno, int protocol)
	{
		struct sockaddr_in	serv_addr = {};

		if (this->isOpen())
			throw AlreadyOpenedException("This socket is already opened");

		/* fill in the structure */
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(portno);
		serv_addr.sin_addr.s_addr = ip;

		/* create the socket */
		this->_socket = socket(AF_INET, SOCK_STREAM, protocol);
		if (this->_socket == INVALID_SOCKET)
			throw SocketCreationErrorException(strerror(errno));

		/* connect the socket */
		if (::connect(this->_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			closesocket(this->_socket);
			this->_socket = INVALID_SOCKET;
			throw ConnectException(std::string("Cannot connect to ") + inet_ntoa(serv_addr.sin_addr));
		}
		this->_opened = true;
	}

	void Socket::disconnect()
	{
		if (!this->isOpen())
			throw NotConnectedException("This socket is not opened");
		closesocket(this->_socket);
		this->_opened = false;
	}

	SOCKET Socket::getSocket()
	{
		return this->_socket;
	}

	void Socket::setSocket(SOCKET sock, bool connected)
	{
		if (this->isOpen())
			throw AlreadyOpenedException("This socket is already opened");

		this->_socket = sock;
		this->_opened = connected;
	}

	std::string Socket::read(int size, int timeout)
	{
		std::string	buf;
		char		buffer[1024];

		while (size != 0) {
			this->waitToBeReady(timeout);

			int bytes = recv(this->_socket, buffer, (static_cast<unsigned>(size) >= sizeof(buffer)) ? (sizeof(buffer)) : (size), 0);

			if (bytes <= 0) {
				if (size < 0)
					break;
				throw EOFException(bytes ? strerror(errno) : "The connection was closed");
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

		while (pos < msg.length()) {
			int bytes = ::send(this->_socket, &msg.c_str()[pos], msg.length() - pos, 0);

			if (bytes <= 0)
				throw EOFException(bytes ? strerror(errno) : "The connection was closed");
			pos += bytes;
		}
	}

	std::string Socket::readUntilEOF(int timeout)
	{
		return this->read(-1, timeout);
	}

	void Socket::waitToBeReady(int timeout)
	{
		FD_SET	set;
		timeval time = {timeout, 0};

		FD_ZERO(&set);
		FD_SET(this->_socket, &set);

		int found = select(FD_SETSIZE, &set, nullptr, nullptr, (timeout > 0 ? &time : nullptr));

		if (found == 0)
			throw TimeoutException("Connection timed out after " + std::to_string(timeout) + " second(s)");
	}

	bool	Socket::isOpen()
	{
		FD_SET	set;
		timeval time = {0, 0};

		FD_ZERO(&set);
		FD_SET(this->_socket, &set);
		if (this->_opened && select(FD_SETSIZE, &set, nullptr, nullptr, &time) == -1)
			this->_opened = false;
		return (this->_opened);
	}
}
