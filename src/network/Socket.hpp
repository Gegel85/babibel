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
    //! @brief implémentation custom des sockets.
	class Socket {
	public:
		explicit Socket();
		Socket(SOCKET sock, bool connected);
		//! @brief Destructeur
		//! @details Ferme automatiquement le Socket quand il est détruit
		~Socket();
		//! @brief renvoie un `bool` pour savoir si le socket est ouvert.
		bool			isOpen();
		//! @brief Ouvre une connection avec un nom d'hote.
		//! @details Cette méthode récupère l'adresse ip de l'hote, puis appelle this->connect(ip_adress, portno, protocol)
		//! @param host Nom de l'host
		//! @param portno Numéro du port
		//! @param protocol protocol à utiliser
		//! @throw AlreadyOpenedException
		//! @throw HostNotFoundException
		virtual void		connect(const std::string &host, unsigned short portno, int protocol = IPPROTO_TCP);
        //! @brief Ouvre une connection avec un nom d'hote.
        //! @param ip Adresse IP du destinataire
        //! @param portno Numéro du port
        //! @param protocol protocol à utiliser
        //! @throw AlreadyOpenedException
        //! @throw SocketCreationErrorException
        //! @throw ConnectException
		virtual void		connect(unsigned int ip, unsigned short portno, int protocol = IPPROTO_TCP);
		//! @brief Déconnecte le socket.
		//! @throw NotConnectedException
		virtual void		disconnect();
		//! @brief Envoie des données au destinataire
		//! @throw EOFException
		virtual void		send(const std::string &);
		//! @brief Lis les données reçu depuis le destinataire.
		//! @param size Taille des données à lire, -1 pour lire l'intégralité.
		//! @param timeout temps maximum d'attente avant
		//! @return Données lu dans le Socket.
		//! @throw TimeoutException
		//! @throw EOFException
		virtual std::string	read(int size, int timeout = -1);
		//! @brief Lis toutes les données reçu depuis le destinataire.
		//! @note Équivaut à Socket.read(-1, timeout).
		virtual std::string	readUntilEOF(int timeout = -1);
		//! @brief Renvoie le socket systeme
		//! @return socket systeme unix ou windows
		SOCKET			getSocket();
		//! @brief Lie un socket systeme à l'objet.
		//! @throw AlreadyOpenedException
		void			setSocket(SOCKET sock, bool connected);
		//! @brief Attend que le socket soit pret en lecture.
		//! @param timeout Temps d'attente avant de lever l'exception TimeoutException
		//! @note si une valeur négative est envoyé, le Socket attend indéfiniment qu'il soit pret.
		//! @throw TimeoutException
		void			waitToBeReady(int timeout);

		//! @brief Renvoie la dernière erreur survenu du Socket.
		//! @return Chaine de caractère contenant la dernière erreur survenu.
		//! @warning La valeur est sucsptible de changer si d'autre appel système on était fait entre la dernière erreur du socket et l'appel de cette fonction !
		static std::string	getLastSocketError();

	protected:
		SOCKET	_socket = INVALID_SOCKET;
		bool	_opened = false;
	};
}

#endif //DISC_ORD_SOCKET_HPP
