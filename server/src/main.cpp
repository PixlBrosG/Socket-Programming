#include <asio.hpp>
#include <iostream>

#include "SmartTV.h"

constexpr uint16_t DEFAULT_SERVER_PORT = 1238;

int main(int argc, char* argv[])
{
	try
	{
		uint16_t port = DEFAULT_SERVER_PORT;
		if (argc > 1)
			port = std::stoi(argv[1]);

		asio::io_context io;
		asio::ip::tcp::acceptor acceptor(io,
			asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

		SmartTV::SmartTV tv(20);
		std::cout << "SmartTV server listening on port " << port << "..." << std::endl;

		for (;;)
		{
			asio::ip::tcp::socket socket(io);
			acceptor.accept(socket);
			std::cout << "Client connected: " << socket.remote_endpoint() << std::endl;

			asio::streambuf buffer;
			std::istream input(&buffer);

			while (true)
			{
				asio::error_code ec;
				size_t n = asio::read_until(socket, buffer, '\n', ec);
				if (ec)
				{
					std::cout << "Client disconnected: " << socket.remote_endpoint() << std::endl;
					break;
				}

				std::string line;
				std::getline(input, line);

				std::string response = tv.HandleCommand(line);
				asio::write(socket, asio::buffer(response), ec);
				if (ec) break;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}