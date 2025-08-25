#pragma once
#include "../command_interpreter.hpp"
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class NewGroup : public Command {
	public:
		NewGroup(CommandInterpreter* interpreter, const std::string& groupName) : m_interpreter(interpreter), m_group_name(groupName) {}
		virtual ~NewGroup() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		CommandInterpreter* m_interpreter;
		std::string m_group_name;
	};
}