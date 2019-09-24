#include <iostream>
#include <boost/asio/io_service.hpp>
#include "TcpServer.hpp"

int	main()
{
	TcpServer	server{10800};

	try {
		server.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
