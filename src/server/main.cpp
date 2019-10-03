#include <iostream>
#include "TcpServer.hpp"

int	main(int argc, char **argv)
{

	if (argc < 2) {
		std::cout << argv[0] << " <port>" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		unsigned		port = std::stoi(argv[1]);

		if (port > 65535)
			throw std::invalid_argument("Port is not in range 0 - 65535");

		BabelServer::TcpServer	server(port);

		server.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
