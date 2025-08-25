#include "set_freq.hpp"

using CommandSystem::Commands::SetFreq;

void SetFreq::Execute(GroupStatus& status) {
	status.noteFrequency = m_freq;
}