#pragma once

#include <asio.hpp>

#include <set>
#include <mutex>
#include <memory>

namespace SmartTV {

	class Session;

	class ClientManager
	{
	public:
		void Add(std::shared_ptr<Session> session);
		void Remove(std::shared_ptr<Session> session);
		void Broadcast(const std::string& message, std::shared_ptr<Session> exclude = nullptr);
	private:
		std::mutex m_Mutex;
		std::set<std::shared_ptr<Session>> m_Sessions;
	};

}