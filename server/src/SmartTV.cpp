#include "SmartTV.h"

#include <sstream>
#include <algorithm>

namespace SmartTV {

	SmartTV::SmartTV(size_t channels)
		: m_On(false), m_ChannelCount(channels), m_CurrentChannel(1)
	{
		if (channels == 0)
			m_ChannelCount = 10;
	}

	bool SmartTV::SetChannel(size_t channel)
	{
		if (!m_On) return false;
		if (channel < 1 || channel > m_ChannelCount) return false;
		m_CurrentChannel = channel;
		return true;
	}

	std::string SmartTV::HandleCommand(const std::string& command)
	{
		// Tokenize
		std::istringstream iss(command);
		std::string cmd;
		iss >> cmd;

		std::ranges::transform(cmd, cmd.begin(), ::toupper);

		if (cmd == "ON")
		{
			TurnOn();
			return "OK\n";
		}

		if (cmd == "OFF")
		{
			TurnOff();
			return "OK\n";
		}

		if (cmd == "GET_CHANNEL")
		{
			if (!IsOn()) return "ERROR tv_off\n";
			return "CHANNEL " + std::to_string(GetCurrentChannel()) + '\n';
		}

		if (cmd == "GET_CHANNELS")
		{
			if (!IsOn()) return "ERROR tv_off\n";
			return "CHANNELS " + std::to_string(GetChannelCount()) + '\n';
		}

		if (cmd == "SET_CHANNEL")
		{
			if (!IsOn()) return "ERROR tv_off\n";

			std::string arg;
			if (!(iss >> arg)) return "ERROR missing_argument\n";

			try {
				size_t newChannel = 0;

				if (arg.starts_with('+') || arg.starts_with('-')) {
					// Relative offset
					int offset = std::stoi(arg);
					newChannel = m_CurrentChannel + offset;
				} else {
					// Absolute channel
					newChannel = std::stoi(arg);
				}

				if (!SetChannel(newChannel))
					return "ERROR invalid_channel\n";

				return "OK\n";
			}
			catch (const std::exception&) {
				return "ERROR invalid_argument\n";
			}
		}


		return "ERROR invalid_command\n";
	}

}
