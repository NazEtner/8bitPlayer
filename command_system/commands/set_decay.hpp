#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetDecay : public Command {
	public:
		SetDecay(uint16_t decay) : m_decay(decay) {}
		virtual ~SetDecay() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		uint16_t m_decay;
	};
}