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

namespace Babel::Network
{
	//! @brief Implémentation custom des sockets.
	class Socket {
	public:
		explicit Socket();
		Socket(SOCKET sock, unsigned remoteIp);
		Socket(Socket &) = delete;

		//! @brief Destructeur
		//! @details Ferme automatiquement le Socket quand il est détruit.
		~Socket();

		//! @brief Renvoie un booleen pour savoir si le Socket est connecté.
		bool	isOpen() const;

		//! @brief Ouvre une connection avec un nom d'hote.
		//! @details Cette méthode récupère l'adresse ip de l'hote, puis appelle connect(ip_address, portno, protocol).
		//! @param host Nom de l'host.
		//! @param portno Numéro du port.
		//! @param protocol Protocole à utiliser.
		//! @throw AlreadyOpenedException
		//! @throw HostNotFoundException
		virtual void	connect(const std::string &host, unsigned short portno, int protocol = IPPROTO_TCP);

		//! @brief Ouvre une connection avec une adresse ip.
		//! @param ip Adresse IP du destinataire.
		//! @param portno Numéro du port.
		//! @param protocol Protocole à utiliser.
		//! @throw AlreadyOpenedException
		//! @throw SocketCreationErrorException
		//! @throw ConnectException
		virtual void	connect(unsigned int ip, unsigned short portno, int protocol = IPPROTO_TCP);

		//! @brief Déconnecte le Socket.
		//! @throw NotConnectedException
		virtual void	disconnect();

		//! @brief Envoie des données au destinataire.
		//! @throw EOFException
		virtual void	send(const std::string &);

		//! @brief Lis les données reçus depuis le destinataire.
		//! @param size Taille des données à lire, -1 pour lire jusqu'à fermeture de la connection.
		//! @param timeout Temps maximum d'attente avant de lever TimeoutException.
		//! @return Données lues dans le Socket.
		//! @throw TimeoutException
		//! @throw EOFException
		virtual std::string	read(int size, int timeout = -1);

		//! @brief Lis toutes les données reçues depuis le destinataire jusqu'à fermeture de la connection.
		//! @note Équivaut à Socket.read(-1, timeout).
		virtual std::string	readUntilEOF(int timeout = -1);

		//! @brief Renvoie le SOCKET système.
		//! @return SOCKET système Unix ou Windows.
		SOCKET	getSocket() const;

		//! @brief Lie un SOCKET système à l'objet.
		//! @param sock SOCKET système.
		//! @param remoteIp ip à laquel le socket est actuellement connecté
		//! @throw AlreadyOpenedException
		void	setSocket(SOCKET soc, unsigned remoteIp = 0);

		//! @brief Attend que le Socket soit prêt en lecture.
		//! @param timeout Temps d'attente avant de lever l'exception TimeoutException.
		//! @note Si une valeur négative est envoyée, le Socket attend jusqu'à ce qu'il soit prêt.
		//! @throw TimeoutException
		void	waitToBeReady(int timeout) const;

		//! @brief Renvoie la dernière erreur survenue du Socket.
		//! @return Chaîne de caractères contenant la dernière erreur survenue.
		//! @warning La valeur est sucéptible de changer si d'autres appels systèmes ont été fait entre la dernière erreur du Socket et l'appel de cette fonction !
		static std::string	getLastSocketError();

		unsigned getRemoteIp() const;

	protected:
		mutable SOCKET	_socket = INVALID_SOCKET;
		unsigned	_remoteIp = 0;
	};
}

#endif //DISC_ORD_SOCKET_HPP
