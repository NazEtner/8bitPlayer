#include "set_decay.hpp"

using CommandSystem::Commands::SetDecay;

void SetDecay::Execute(GroupStatus& status) {
	status.decayPerTick = m_decay;
}