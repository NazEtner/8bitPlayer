#include "set_note_ticks.hpp"

using CommandSystem::Commands::SetNoteTicks;

void SetNoteTicks::Execute(GroupStatus& status) {
	status.noteTicks = m_ticks;
}