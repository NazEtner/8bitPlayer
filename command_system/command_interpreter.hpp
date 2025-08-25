#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "command_group.hpp"
#include "../audio/player.hpp"

namespace CommandSystem {
	class CommandInterpreter {
	public:
		CommandInterpreter();
		virtual ~CommandInterpreter() = default;
		void AddCommand(const std::string& command);
		void NewCommandGroup(const std::string& groupName);
		void SetCurrentCommandGroup(const std::string& groupName, const size_t id);
		void ExecuteCommandGroup(const std::string& groupName);
		void ExecuteCurrentCommandGroup();
		void ClearCommandBuffer();
		void Play(const std::vector<std::string>& groupNames);
		bool ShouldExit() const { return m_should_exit; }
	private:
		// 同名のコマンドグループで生成された音声を同時に再生したい
		std::unordered_map<std::string, std::vector<std::shared_ptr<CommandGroup>>> m_command_groups;
		std::shared_ptr<CommandGroup> m_current_command_group;

		bool m_should_exit = false;

		void m_parse_command_line(const std::string& line);

		Audio::Player m_player;
	};
}