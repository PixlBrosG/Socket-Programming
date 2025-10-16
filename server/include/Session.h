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
			: m_Socket(std::move(socket)), m_TV(tv), m_Manager(manager) {}

		void Start();
		void Deliver(const std::string& message);
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
	};

}