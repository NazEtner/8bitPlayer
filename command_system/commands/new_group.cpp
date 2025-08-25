#include "new_group.hpp"

using CommandSystem::Commands::NewGroup;

void NewGroup::Execute(GroupStatus& status) {
	m_interpreter->NewCommandGroup(m_group_name);
}