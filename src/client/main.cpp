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

namespace Babel::Client
{
	void handleConnection(Network::Socket &socket, const std::string &ip, unsigned short port, bool &end)
	{
		try {
			Network::Protocol::Packet packet;

			packet.op = Network::Protocol::HELLO;
			packet.data = VERSION_STR;
			socket.connect(ip, port);
			socket.send(packet);
			while (socket.isOpen() && !end) {
				try {
					socket.waitToBeReady(1);
				} catch (Network::Exceptions::TimeoutException &) {
					continue;
				}
				packet = socket;
				std::cout << "Server sent packet " << packet << std::endl;
			}
		} catch (std::exception &e) {
			std::cerr << "An error occurred and the connection to the server will be interrupted: " << e.what() << std::endl;
			if (socket.isOpen())
				socket.disconnect();
		}
	}

	int babel(std::string ip, unsigned short port, int argc, char **argv)
	{
		bool end = false;
		Network::Socket socket;
		std::thread clientThread{
			[&socket, &ip, &port, &end](){
				while (!end) {
					handleConnection(socket, ip, port, end);
					if (end)
						break;
					std::cout << "Reconnecting in 10 seconds..." << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(10));
				}
			}
		};
		QTApplication app(argc, argv);
		BabelQTClient qtClient(socket, {1000, 500});

		qtClient.window.show();

		int code = app.launch();

		end = true;
		if (clientThread.joinable())
			clientThread.join();
		return code;
	}
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cout << argv[0] << ": <serverIp> <port>" << std::endl;
		return EXIT_FAILURE;
	}

	unsigned int port;
	bool end = false;

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

	return Babel::Client::babel(argv[1], port, argc, argv);
}