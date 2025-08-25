#include "set_duty_cycle.hpp"

using CommandSystem::Commands::SetDutyCycle;

void SetDutyCycle::Execute(GroupStatus& status) {
	status.dutyCycle = m_duty;
}