#include "SmartTV/ClientManager.h"

#include "SmartTV/Log.h"
#include "SmartTV/Session.h"

namespace SmartTV {

	void ClientManager::Add(std::shared_ptr<Session> session)
	{
		std::lock_guard lock(m_Mutex);
		m_Sessions.insert(session);

		Logger::Server::Info("New connection from {}:{}", session->GetAddress(), session->GetPort());
	}

	void ClientManager::Remove(std::shared_ptr<Session> session)
	{
		std::lock_guard lock(m_Mutex);
		m_Sessions.erase(session);

		Logger::Server::Info("Connection closed ({}:{})", session->GetAddress(), session->GetPort());
	}

	void ClientManager::Broadcast(const std::string& message, std::shared_ptr<Session> exclude)
	{
		std::lock_guard lock(m_Mutex);
		for (auto& s : m_Sessions)
			if (s != exclude)
				s->Deliver(message);
	}

}