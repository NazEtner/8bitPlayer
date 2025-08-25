#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetBPM : public Command {
	public:
		SetBPM(uint16_t bpm) : m_bpm(bpm) {}
		virtual ~SetBPM() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		uint16_t m_bpm;
	};
}