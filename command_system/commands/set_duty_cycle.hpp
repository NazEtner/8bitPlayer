#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetDutyCycle : public Command {
	public:
		SetDutyCycle(double duty) : m_duty(duty) {}
		virtual ~SetDutyCycle() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		double m_duty;
	};
}