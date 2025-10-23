#pragma once

#include <asio.hpp>

#include <memory>
#include <deque>
#include <iostream>

#include "SmartTV.h"
#include "ClientManager.h"

namespace SmartTV {

	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		Session(asio::ip::tcp::socket socket, SmartTV& tv, ClientManager& manager)
			: m_Socket(std::move(socket)), m_TV(tv), m_Manager(manager)
		{
			m_Address = m_Socket.remote_endpoint().address().to_string();
			m_Port = m_Socket.remote_endpoint().port();
		}

		void Start();
		void Deliver(const std::string& message);

		const std::string& GetAddress() const { return m_Address; }
		uint16_t GetPort() const { return m_Port; }
	private:
		void DoRead();
		void OnRead(asio::error_code ec, std::size_t bytesTransferred);
		void DoWrite();
	private:
		asio::ip::tcp::socket m_Socket;
		asio::streambuf m_ReadBuffer;
		std::deque<std::string> m_WriteQueue;
		SmartTV& m_TV;
		ClientManager& m_Manager;

		std::string m_Address;
		std::uint16_t m_Port;
	};

}