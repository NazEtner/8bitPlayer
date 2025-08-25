#include "run.hpp"

using CommandSystem::Commands::Run;

void Run::Execute(GroupStatus& status) {
	m_interpreter->ExecuteCommandGroup(m_group_name);
}