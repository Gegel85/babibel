/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main.cpp
*/

#include <thread>
#include <iostream>
#include "../network/Socket.hpp"
#include "graphical/qt5/BabelQTClient.hpp"
#include "graphical/qt5/QTApplication.hpp"
#include "../network/Protocol.hpp"
#include "../network/SocketExceptions.hpp"
#include "TcpClient.hpp"
#include "sound/Sound.hpp"

namespace Babel::Client
{
	int babel(const std::string &ip, unsigned short port, int argc, char **argv)
	{
		TcpClient client;
		QTApplication app(argc, argv);
		BabelQTClient qtClient(client, {1000, 500});
		int code = EXIT_FAILURE;

		try {
			client.connectToServer(ip, port, 10000);
			qtClient.window.show();

			code = app.launch();

			client.disconnect();
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		return code;
	}
}

int main(int argc, char **argv)
{
	unsigned int port;
	std::string ip;

	if (argc != 3) {
		std::cout << argv[0] << ": <serverIp> <port>" << std::endl;
		std::cout << "Assuming server is on localhost:10800..." << std::endl;
		ip = "localhost";
		port = 10800;
	} else {
		ip = argv[1];
		try {
			port = std::stoi(argv[2]);
		} catch (std::exception &) {
			std::cerr << "The port number is invalid" << std::endl;
			return EXIT_FAILURE;
		}

		if (port > 65535) {
			std::cerr << "The port number is invalid" << std::endl;
			return EXIT_FAILURE;
		}
	}

	return Babel::Client::babel(ip, port, argc, argv);
}