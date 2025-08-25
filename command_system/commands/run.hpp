#pragma once
#include "../command_interpreter.hpp"
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class Run : public Command {
	public:
		Run(CommandInterpreter* interpreter, const std::string& groupName) : m_interpreter(interpreter), m_group_name(groupName) {}
		virtual ~Run() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		CommandInterpreter* m_interpreter;
		std::string m_group_name;
	};
}