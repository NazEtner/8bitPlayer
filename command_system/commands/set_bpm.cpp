#include "set_bpm.hpp"

using CommandSystem::Commands::SetBPM;

void SetBPM::Execute(GroupStatus& status) {
	status.bpm = m_bpm;
}
