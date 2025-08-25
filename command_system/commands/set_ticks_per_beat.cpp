#include "set_ticks_per_beat.hpp"

using CommandSystem::Commands::SetTicksPerBeat;

void SetTicksPerBeat::Execute(GroupStatus& status) {
	status.ticksPerBeat = m_ticks;
}
