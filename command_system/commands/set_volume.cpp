#include "set_volume.hpp"

using CommandSystem::Commands::SetVolume;

void SetVolume::Execute(GroupStatus& status) {
	status.volume = m_volume;
}
