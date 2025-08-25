#pragma once
#include "command.hpp"
#include "group_status.hpp"
#include <memory>
#include <iostream>

namespace CommandSystem {
	class CommandGroup {
	public:
		CommandGroup(const std::string& name);
		virtual ~CommandGroup() = default;
		void AddCommand(std::shared_ptr<Command> command);
		void Execute();
		const GroupStatus& GetStatus() const;
		void PrintInfo() const {
			std::cout << "Command Group: " << m_status.groupName << ", Commands: " << m_commands.size() << std::endl;
			std::cout << "BPM: " << m_status.bpm << ", Volume: " << (int)m_status.volume << std::endl;
			std::cout << "Waveform: ";
			switch (m_status.waveform) {
			case GroupStatus::eWaveform::Square:
				std::cout << "Square";
				break;
			case GroupStatus::eWaveform::Triangle:
				std::cout << "Triangle";
				break;
			case GroupStatus::eWaveform::Sine:
				std::cout << "Sine";
				break;
			case GroupStatus::eWaveform::Noise:
				std::cout << "Noise";
				break;
			default:
				std::cout << "Unknown";
				break;
			}
			std::cout << std::endl;
		}
	private:
		std::vector<std::shared_ptr<Command>> m_commands;
		GroupStatus m_status;
	};
}