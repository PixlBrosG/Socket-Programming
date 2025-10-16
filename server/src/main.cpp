#include <asio.hpp>
#include <iostream>

#include "ClientManager.h"
#include "Session.h"
#include "SmartTV.h"

constexpr uint16_t DEFAULT_SERVER_PORT = 1238;

using asio::ip::tcp;

class Server
{
public:
	Server(asio::io_context& io, uint16_t port)
		: m_Acceptor(io, tcp::endpoint(tcp::v4(),port)), m_Tv(10)
	{
		Accept();
	}
private:
	void Accept()
	{
		m_Acceptor.async_accept([this](asio::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				auto session = std::make_shared<SmartTV::Session>(std::move(socket), m_Tv, m_Manager);
				session->Start();
			}
			Accept();
		});
	}
private:
	tcp::acceptor m_Acceptor;
	SmartTV::SmartTV m_Tv;
	SmartTV::ClientManager m_Manager;
};

int main(int argc, char* argv[])
{
	uint16_t port = DEFAULT_SERVER_PORT;
	if (argc > 1)
		port = std::stoi(argv[1]);

	try
	{
		asio::io_context io;
		Server server(io, port);
		std::cout << "SmartTV server listening on port " << port << "..." << std::endl;

		io.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}