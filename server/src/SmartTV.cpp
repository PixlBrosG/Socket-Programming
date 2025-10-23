#include "SmartTV/SmartTV.h"

#include <sstream>
#include <algorithm>

namespace SmartTV {

	SmartTV::SmartTV(int channels)
		: m_TotalChannels(channels)
	{
	}

	void SmartTV::TurnOn() { std::lock_guard lock(m_Mutex); m_On = true; }
	void SmartTV::TurnOff() { std::lock_guard lock(m_Mutex); m_On = false; }
	bool SmartTV::IsOn() const { std::lock_guard lock(m_Mutex); return m_On; }
	uint32_t SmartTV::GetChannel() const { std::lock_guard lock(m_Mutex); return m_CurrentChannel; }

	bool SmartTV::SetChannel(uint32_t channel)
	{
		std::lock_guard lock(m_Mutex);
		if (channel < 1 || channel < m_TotalChannels)
			return false;

		m_CurrentChannel = channel;
		return true;
	}

	SmartTV::Result SmartTV::HandleCommand(const std::string& command)
	{
		std::istringstream iss(command);
		std::string cmd;
		iss >> cmd;
		std::ranges::transform(cmd, cmd.begin(), ::toupper);

		std::lock_guard lk(m_Mutex);
		Result r{"ERROR invalid_command\n", false};

		if (cmd == "ON")
		{
			m_On = true;
			r.Line = "OK\n";
		}
		else if (cmd == "OFF")
		{
			m_On = false;
			r.Line = "OK\n";
		}
		else if (cmd == "STATUS")
		{
			r.Line = std::string("STATE ") + (m_On ? "ON\n" : "OFF\n");
		}
		else if (cmd == "GET_CHANNELS")
		{
			if (!m_On)
				r.Line = "ERROR tv_off\n";
			else
				r.Line = "CHANNELS " + std::to_string(m_TotalChannels) + "\n";
		}
		else if (cmd == "GET_CHANNEL")
		{
			if (!m_On)
				r.Line = "ERROR tv_off\n";
			else
				r.Line = "CHANNEL " + std::to_string(m_CurrentChannel) + "\n";
		}
		else if (cmd == "SET_CHANNEL") {
			if (!m_On) r.Line = "ERROR tv_off\n";
			else
			{
				if (std::string arg; !(iss >> arg))
				{
					r.Line = "ERROR missing_argument\n";
				}
				else
				{
					try
					{
						uint32_t newCh = 0;
						if (!arg.empty() && (arg[0] == '+' || arg[0] == '-'))
						{
							newCh = m_CurrentChannel + std::stoi(arg);
						}
						else
						{
							newCh = std::stoi(arg);
						}
						if (newCh < 1 || newCh > m_TotalChannels)
						{
							r.Line = "ERROR invalid_channel\n";
						}
						else
						{
							m_CurrentChannel = newCh;
							r.Line = "OK\n";
							r.ChannelChanged = true;
						}
					}
					catch (...)
					{
						r.Line = "ERROR invalid_argument\n";
					}
				}
			}
		}

		return r;
	}
}
