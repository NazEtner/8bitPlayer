#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetNoteTicks : public Command {
	public:
		SetNoteTicks(uint16_t ticks) : m_ticks(ticks) {}
		virtual ~SetNoteTicks() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		uint16_t m_ticks;
	};
}