#include "command_group.hpp"

using CommandSystem::CommandGroup;
using CommandSystem::GroupStatus;

CommandGroup::CommandGroup(const std::string& name) {
	m_status.groupName = name;
}

void CommandGroup::AddCommand(std::shared_ptr<Command> command) {
	m_commands.push_back(command);
}

void CommandGroup::Execute() {
	for (const auto& command : m_commands) {
		command->Execute(m_status);
	}
}

const GroupStatus& CommandGroup::GetStatus() const {
	return m_status;
}