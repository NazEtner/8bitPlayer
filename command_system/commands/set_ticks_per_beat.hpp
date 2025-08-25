#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetTicksPerBeat : public Command {
	public:
		SetTicksPerBeat(uint8_t ticks) : m_ticks(ticks) {}
		virtual ~SetTicksPerBeat() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		uint8_t m_ticks;
	};
}