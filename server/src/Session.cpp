#include "SmartTV/Session.h"

#include "SmartTV/Log.h"

namespace SmartTV {

	void Session::Start()
	{
		m_Manager.Add(shared_from_this());
		asio::async_write(m_Socket, asio::buffer("Hello from SmartTV Server\n"),
			[self=shared_from_this()](asio::error_code, size_t){});
		DoRead();
	}

	void Session::DoRead()
	{
		auto self(shared_from_this());
		asio::async_read_until(m_Socket, m_ReadBuffer, '\n',
			[self](asio::error_code ec, size_t bytesTransferred)
			{
				self->OnRead(ec, bytesTransferred);
			});
	}

	void Session::OnRead(asio::error_code ec, size_t bytesTransferred)
	{
		if (ec)
		{
			Logger::Server::Error("{}", ec.message());
			m_Manager.Remove(shared_from_this());
			return;
		}

		std::istream is(&m_ReadBuffer);
		std::string command;
		std::getline(is, command);
		if (command.empty()) { DoRead(); return; }

		Logger::Server::Info("Received command '{}'", command);

		SmartTV::Result result = m_TV.HandleCommand(command);
		Deliver(result.Line);

		if (result.ChannelChanged)
		{
			std::string msg = "CHANNEL " + std::to_string(m_TV.GetChannel()) + "\n";
			m_Manager.Broadcast(msg, shared_from_this());
		}

		DoRead();
	}

	void Session::Deliver(const std::string& message)
	{
		bool writeInProgress = !m_WriteQueue.empty();
		m_WriteQueue.emplace_back(message);
		if (!writeInProgress)
			DoWrite();
	}

	void Session::DoWrite()
	{
		auto self(shared_from_this());
		asio::async_write(m_Socket, asio::buffer(m_WriteQueue.front()),
			[self](asio::error_code ec, size_t bytesTransferred)
			{
				if (!ec)
				{
					self->m_WriteQueue.pop_front();
					if (!self->m_WriteQueue.empty())
						self->DoWrite();
				}
				else
				{
					self->m_Manager.Remove(self);
				}
			});
	}

}
