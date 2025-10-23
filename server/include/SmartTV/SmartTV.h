#pragma once

#include <string>
#include <cstdint>
#include <mutex>

namespace SmartTV {

	class SmartTV
	{
	public:
		struct Result
		{
			std::string Line;
			bool ChannelChanged;
		};

		explicit SmartTV(int channels = 10);

		Result HandleCommand(const std::string& command);

		// Core logic (thread-safe)
		void TurnOn();
		void TurnOff();
		bool IsOn() const;
		bool SetChannel(uint32_t channel);
		uint32_t  GetChannel() const;
		uint32_t  GetChannelCount() const { return m_TotalChannels; }
	private:
		mutable std::mutex m_Mutex;
		bool m_On = false;
		uint32_t m_TotalChannels = 10;
		uint32_t m_CurrentChannel = 1;
	};

}
