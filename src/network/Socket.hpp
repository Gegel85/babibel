//
// Created by Gegel85 on 05/04/2019.
//

#ifndef DISC_ORD_SOCKET_HPP
#define DISC_ORD_SOCKET_HPP


#ifdef _WIN32
#	include <winsock.h>
#else
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <sys/socket.h>
#	define INVALID_SOCKET -1
	typedef int SOCKET;
#endif
#include <string>

namespace Babel
{
	class Socket {
	public:
		explicit Socket();
		Socket(SOCKET sock, bool connected);
		~Socket();
		bool			isOpen();
		virtual void		connect(const std::string &host, unsigned short portno, int protocol = IPPROTO_TCP);
		virtual void		connect(unsigned int ip, unsigned short portno, int protocol = IPPROTO_TCP);
		virtual void		disconnect();
		virtual void		send(const std::string &);
		virtual std::string	read(int size, int timeout = -1);
		virtual std::string	readUntilEOF(int timeout = -1);
		SOCKET			getSocket();
		void			setSocket(SOCKET sock, bool connected);
		void			waitToBeReady(int timeout);

		static std::string	getLastSocketError();

	protected:
		SOCKET	_socket = INVALID_SOCKET;
		bool	_opened = false;
	};
}

#endif //DISC_ORD_SOCKET_HPP
