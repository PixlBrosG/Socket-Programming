#pragma once

#include <string>
#include <cstdint>

namespace SmartTV {

	class SmartTV
	{
	public:
		explicit SmartTV(size_t channels = 10);

		// State control
		void TurnOn() { m_On = true; }
		void TurnOff() { m_On = false; }
		[[nodiscard]] bool IsOn() const { return m_On; }

		// Channels
		[[nodiscard]] size_t GetChannelCount() const { return m_ChannelCount; }
		[[nodiscard]] size_t GetCurrentChannel() const { return m_CurrentChannel; }
		bool SetChannel(size_t channel);

		// Protocol helper
		std::string HandleCommand(const std::string& command);
	private:
		bool m_On;
		size_t m_ChannelCount;
		size_t m_CurrentChannel;
	};

}
