#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetWaveform : public Command {
	public:
		SetWaveform(const std::string& waveform) : m_waveform(waveform) {}
		virtual ~SetWaveform() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		std::string m_waveform;
	};
}