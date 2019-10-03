/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** main.cpp
*/

#include <thread>
#include <iostream>
#include "../network/Socket.hpp"
#include "graphical/qt5/myQTWindow.hpp"
#include "graphical/qt5/QTApplication.hpp"
#include "../network/Protocol.hpp"
#include "../network/SocketExceptions.hpp"

namespace Babel
{
	int babel(std::string ip, unsigned short port, int argc, char **argv)
	{
		bool end = false;
		Babel::Socket socket;
		std::thread clientThread{
			[&socket, &ip, &port, &end](){
				while (!end) {
					try {
						Babel::Protocol::Packet packet;

						packet.op = Protocol::HELLO;
						packet.data = VERSION_STR;
						socket.connect(ip, port);
						socket.send(packet);
						while (socket.isOpen() && !end) {
							try {
								socket.waitToBeReady(1);
							} catch (TimeoutException &) {
								continue;
							}
							packet = socket;
							std::cout << "Server sent packet " << packet << std::endl;
						}
						break;
					} catch (std::exception &e) {
						std::cerr << "An error occurred and the connection to the server will be interrupted: " << e.what() << std::endl;
						if (socket.isOpen())
							socket.disconnect();
					}
					std::cout << "Reconnecting in 10 seconds..." << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(10));
				}
			}
		};
		Babel::QTApplication app(argc, argv);
		Babel::myQTWindow myWindow({1000, 500});

		myWindow.window.show();

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

	return Babel::babel(argv[1], port, argc, argv);
}

/*
int main(int argc, char **argv)
{
	Babel::TempWindow	window{{640, 480}};

	do {
		//Some stuff
	} while (window.refresh());
}
*/

/*
namespace Babel
{
	class TempWindow : public GUIScreen {
	private:
		Vector2<unsigned int> _size;

	public:
		TempWindow(Vector2<unsigned int> size) : GUIScreen(), _size{size} {};

		Vector2<unsigned int> getSize() const override
		{
			return this->_size;
		};

		virtual void setFullscreen(bool fullscreen = true) override
		{
			std::cout << "Fullscreen is now " << (fullscreen ? "on" : "off") << std::endl;
		};

		virtual void setSize(Vector2<unsigned int> size)
		{
			std::cout << "New window size (" << size.x << ", " << size.y << ")" << std::endl;
		};

		virtual bool refresh()
		{
			std::cout << "Rendering objects" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(17));
			return true;
		};
	};
}
*/