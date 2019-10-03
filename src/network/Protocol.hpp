/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Protocol.hpp
*/

#ifndef CPP_BABEL_2019_PROTOCOL_HPP
#define CPP_BABEL_2019_PROTOCOL_HPP


#include <vector>
#include <random>
#include <string>
#include "Socket.hpp"

namespace BabelNetwork::Protocol
{
	//! @brief Version actuelle du protocole.
	#define VERSION_STR std::string{'\0', '\x01'}

	//! @brief Code réprésentant l'opération que le client/serveur veut effectuer.
	//! @note Une taille `d` indique que la taille est dynamique et doit être trouvé dans le header du Packet.
	//! @warning Si la taille attendue ne correspond pas à la taille donnée dans le header, une erreur BAD_PACKET sera levée.
	enum Opcode {
		//! taille: d | Code générique en cas de succès.
		OK,

		//! taille: d | Code générique en cas d'erreur.
		KO,

		//! taille: 2 | Initialisation du socket. Envoie en data la version du protocole.
		HELLO,

		//! taille: 4 | Fermeture du socket. Envoie en data le code retour. (voir: ErrorReason)
		BYE,

		//! taille: 64| (Client) Connecte le client. Envoie en data le nom d'utilisateur et le mot de passe de longueur 32 chacun. (Remplis avec des bytes nuls)
		LOGIN,

		//! taille: 0 | (Client) Déconnecte le client.
		LOGOUT,

		//! taille: 64| (Client) Enregistre le client. Même data que pour le REGISTER.
		REGISTER,

		//! taille: d | (Client) Demande la liste d'amis au serveur. Envoie une liste d'ID de chacun 4 octets.
		GET_FRIENDS,

		//! taille: d | (Client) Renvoie une structure de donnée `User`.
		//! @code
		//!   struct User {
		//!	   unsigned int id;
		//!	   char username[32];
		//!   };
		//! @endcode
		GET_USER_INFOS,

		//! taille: 4 | Envoie au (ou reçoit du) serveur une demande d'appel avec l'ID de l'utilisateur que le client souhaite contacter.
		CALL,

		//! taille: 6 | Appel accepté. Renvoie une structure `Call`.
		//! @code
		//!   struct Call {
		//!	   unsigned int ip_adress;
		//!	   unsigned short port;
		//!   };
		//! @endcode
		//! @note Si l'ip envoyée est 0.0.0.0, le client est l'hôte doit attendre la connexion du partenaire.
		CALL_REFUSED,

		//! taille: 0 | Appel refusé.
		CALL_ACCEPTED,
	};

	namespace ErrorReason {
		//! Fermeture normale de la connection.
		extern std::string NORMAL_CLOSURE;

		//! Le format du paquet reçu ne correspond pas à celui qui était attendu.
		extern std::string BAD_PACKET;

		//! La version du protocol du client ne correspond pas à celle du serveur.
		extern std::string BAD_VERSION;

		//! L'`Opcode` reçu est inconnu.
		extern std::string BAD_OPCODE;

		//! Le nom d'utilisateur ou le mot de passe est incorrect.
		extern std::string BAD_CREDENTIALS;

		//! L'utilisateur est déjà connecté.
		extern std::string ALREADY_CONNECTED;

		//! La ressource est déjà utilisé par un autre client.
		extern std::string ALREADY_USED;

		//! Le client a déjà lancé un appel.
		extern std::string ALREADY_CALLING;

		//! Le client n'est pas authentifié.
		extern std::string NOT_CONNECTED;

		//! Le client n'a pas l'autorisation d'accéder à la ressource demandée.
		extern std::string NOT_AUTHORIZED;

		//! La ressource demandée n'a pas été trouvé.
		extern std::string NOT_FOUND;

		//! Le destinataire n'est pas connecté.
		extern std::string REMOTE_NOT_CONNECTED;
	}

	class InvalidPacketException : public std::exception {
	private:
		std::string _msg;

	public:
		InvalidPacketException(const std::string &msg) : _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};


	//! @note Pour plus d'information sur la structure des packet, voir Protocol.
	//! @warning Si la taille attendue ne correspond pas à la taille donnée dans le header, une erreur BAD_PACKET sera levée.
	class Packet {
	private:
		//! RNG pour générer la clé.
		std::random_device _random;

	public:
		//! Protocol::Opcode envoyé.
		unsigned char op;

		//! Données reçues. (voir la documentation Protocol::Opcode)
		std::string data;

		Packet() = default;
		Packet(Socket &);
		Packet(const std::string &);
		Packet &operator=(Socket &);
		Packet &operator=(const std::string &);
		operator std::string();

		//! @brief Permet de changer un entier non signé en une chaîne de charactère de 4 bytes.
		//! @param value	Valeur à changer
		//! @return		 Une chaîne de 4 caractères réprésentant la value, en MSBF (Most significant byte first)
		static std::string	toByteString(unsigned value);

		//! @brief Permet de transformer une chaîne de charactère de 4 bytes codée en MSBF en entier non signé.
		//! @param str  Chaîne de caractère à transformer
		//! @return	 Un entier non signé réprésentant la chaine de caractère
		static unsigned		uintFromByteString(const std::string &str);
	};
}

std::ostream &operator<<(std::ostream &stream, BabelNetwork::Protocol::Packet &packet);


#endif //CPP_BABEL_2019_PROTOCOL_HPP
