#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetFreq : public Command {
	public:
		SetFreq(double freq) : m_freq(freq) {}
		virtual ~SetFreq() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		double m_freq;
	};
}