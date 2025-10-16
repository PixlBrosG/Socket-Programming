#include <asio.hpp>
#include <iostream>
#include <csignal>
#include <cstdlib>

#include "ClientManager.h"
#include "Session.h"
#include "SmartTV.h"

constexpr uint16_t DEFAULT_SERVER_PORT = 1238;

using asio::ip::tcp;

// --- Global pointers for cleanup handlers ---
SmartTV::ClientManager* g_Manager = nullptr;
asio::io_context* g_IOContext = nullptr;

// --- Signal handler for Ctrl+C or SIGTERM ---
void SignalHandler(int signal)
{
	std::cout << "\n[Signal] Received " << signal << " - shutting down gracefully" << std::endl;
	if (g_Manager) g_Manager->Broadcast("SERVER_OFFLINE reason=signal\n");
	if (g_IOContext) g_IOContext->stop();
}

// --- Normal exit (atexit) ---
void OnExit()
{
	if (g_Manager)
	{
		std::cout << "\n[Exit] Broadcasting SERVER_OFFLINE before shutdown" << std::endl;
		g_Manager->Broadcast("SERVER_OFFLINE reason=exit\n");
	}
}

class Server
{
public:
	Server(asio::io_context& io, uint16_t port)
		: m_Acceptor(io, tcp::endpoint(tcp::v4(),port)), m_Tv(10)
	{
		Accept();
	}

	SmartTV::ClientManager& GetManager() { return m_Manager; }
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

		// Register globals from signal/exit handling
		g_IOContext = &io;
		g_Manager = &server.GetManager();

		// Register handlers
		std::signal(SIGINT, SignalHandler);
		std::signal(SIGTERM, SignalHandler);
		std::atexit(OnExit);

		std::cout << "SmartTV server listening on port " << port << "..." << std::endl;
		io.run();

		std::cout << "\n[Server] Clean exit" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n[Error]" << e.what() << std::endl;
	}
}