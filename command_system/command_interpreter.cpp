#include "command_interpreter.hpp"
#include <sstream>
#include <fstream>

// commands include
#include "commands/run.hpp"
#include "commands/set_bpm.hpp"
#include "commands/set_decay.hpp"
#include "commands/set_duty_cycle.hpp"
#include "commands/set_freq.hpp"
#include "commands/set_note_ticks.hpp"
#include "commands/set_pitch.hpp"
#include "commands/set_ticks_per_beat.hpp"
#include "commands/set_volume.hpp"
#include "commands/set_waveform.hpp"
#include "commands/note.hpp"
#include "commands/new_group.hpp"

using CommandSystem::CommandInterpreter;

CommandInterpreter::CommandInterpreter() {
	ClearCommandBuffer();
}

void CommandInterpreter::AddCommand(const std::string& command) {
	std::istringstream stream(command);
	std::string line;
	while (std::getline(stream, line)) {
		m_parse_command_line(line);
	}
}

void CommandInterpreter::m_parse_command_line(const std::string& line) {
	std::istringstream stream(line);
	std::string commandName;
	stream >> commandName;
	if (commandName == "run") {
		std::string groupName;
		if (!(stream >> groupName)) {
			groupName = "__anonymous__";
		}
		auto runCommand = std::make_shared<CommandSystem::Commands::Run>(this, groupName);
		m_current_command_group->AddCommand(runCommand);
	}
	else if (commandName == "setBPM") {
		uint16_t bpm;
		stream >> bpm;
		auto setBPMCommand = std::make_shared<CommandSystem::Commands::SetBPM>(bpm);
		m_current_command_group->AddCommand(setBPMCommand);
	}
	else if (commandName == "setTicksPerBeat") {
		uint16_t ticks;
		stream >> ticks;
		auto setTicksPerBeatCommand = std::make_shared<CommandSystem::Commands::SetTicksPerBeat>(ticks);
		m_current_command_group->AddCommand(setTicksPerBeatCommand);
	}
	else if (commandName == "setNoteTicks") {
		uint16_t ticks;
		stream >> ticks;
		auto setNoteTicksCommand = std::make_shared<CommandSystem::Commands::SetNoteTicks>(ticks);
		m_current_command_group->AddCommand(setNoteTicksCommand);
	}
	else if (commandName == "setVolume") {
		uint16_t volume;
		stream >> volume;
		auto setVolumeCommand = std::make_shared<CommandSystem::Commands::SetVolume>(volume);
		m_current_command_group->AddCommand(setVolumeCommand);
	}
	else if (commandName == "setPitch") {
		std::string pitch;
		stream >> pitch;
		auto setPitchCommand = std::make_shared<CommandSystem::Commands::SetPitch>(pitch);
		m_current_command_group->AddCommand(setPitchCommand);
	}
	else if (commandName == "setFreq") {
		double freq;
		stream >> freq;
		auto setFreqCommand = std::make_shared<CommandSystem::Commands::SetFreq>(freq);
		m_current_command_group->AddCommand(setFreqCommand);
	}
	else if (commandName == "setDutyCycle") {
		double dutyCycle;
		stream >> dutyCycle;
		auto setDutyCycleCommand = std::make_shared<CommandSystem::Commands::SetDutyCycle>(dutyCycle);
		m_current_command_group->AddCommand(setDutyCycleCommand);
	}
	else if (commandName == "setDecay") {
		uint16_t decay;
		stream >> decay;
		auto setDecayCommand = std::make_shared<CommandSystem::Commands::SetDecay>(decay);
		m_current_command_group->AddCommand(setDecayCommand);
	}
	else if (commandName == "newGroup") {
		std::string groupName;
		if (!(stream >> groupName)) {
			return;
		}
		NewCommandGroup(groupName);
	}
	else if (commandName == "setWaveform") {
		std::string waveform;
		stream >> waveform;
		auto setWaveformCommand = std::make_shared<CommandSystem::Commands::SetWaveform>(waveform);
		m_current_command_group->AddCommand(setWaveformCommand);
	}
	else if (commandName == "note") {
		auto noteCommand = std::make_shared<CommandSystem::Commands::Note>();
		m_current_command_group->AddCommand(noteCommand);
	}
	else if (commandName == "exit") {
		m_should_exit = true;
	}
	else if (commandName == "printInfo") {
		m_current_command_group->PrintInfo();
	}
	else if (commandName == "execute") {
		ExecuteCurrentCommandGroup();
	}
	else if (commandName == "play") {
		std::vector<std::string> groupNames;
		std::string groupName;
		while (stream >> groupName) {
			groupNames.push_back(groupName);
		}
		if (groupNames.empty()) {
			groupNames.push_back("__anonymous__");
		}
		Play(groupNames);
	}
	else if (commandName == "clear") {
		ClearCommandBuffer();
	}
	else if (commandName == "load") {
		std::string filename;
		if (stream >> filename) {
			std::ifstream file(filename);
			if (file.is_open()) {
				AddCommand(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
			}
			else {
				std::cerr << "Failed to open file: " << filename << std::endl;
			}
		}
		else {
			std::cerr << "Filename not provided for load command." << std::endl;
		}
	}
	else if (commandName == "" || commandName[0] == '#') {
		// 空行またはコメント行は無視
	}
	else {
		std::cerr << "Unknown command: " << commandName << std::endl;
	}
}

void CommandInterpreter::NewCommandGroup(const std::string& groupName) {
	auto commandGroup = std::make_shared<CommandGroup>(groupName);
	m_command_groups[groupName].push_back(commandGroup);
	m_current_command_group = commandGroup;
}

void CommandInterpreter::SetCurrentCommandGroup(const std::string& groupName, const size_t id) {
	if (m_command_groups.find(groupName) != m_command_groups.end() && id < m_command_groups[groupName].size()) {
		m_current_command_group = m_command_groups[groupName][id];
	}
}

void CommandInterpreter::ExecuteCommandGroup(const std::string& groupName) {
	if (m_command_groups.find(groupName) != m_command_groups.end()) {
		for (const auto& commandGroup : m_command_groups[groupName]) {
			commandGroup->Execute();
		}
	}
}

void CommandInterpreter::ExecuteCurrentCommandGroup() {
	if (m_current_command_group) {
		m_current_command_group->Execute();
	}
}

void CommandInterpreter::ClearCommandBuffer() {
	m_command_groups.clear();
	
	m_command_groups["__anonymous__"].push_back(std::make_shared<CommandGroup>("__anonymous__"));
	m_current_command_group = m_command_groups["__anonymous__"].front();
}

void CommandInterpreter::Play(const std::vector<std::string>& groupNames) {
	uint32_t order = 0;
	for (const auto& groupName : groupNames) {
		if (m_command_groups.find(groupName) != m_command_groups.end()) {
			for (const auto& commandGroup : m_command_groups[groupName]) {
				auto status = commandGroup->GetStatus();
				if (status.soundBuffer.empty()) {
					continue;
				}
				m_player.AddBuffer(order, status.soundBuffer, status.sampleRate);
			}
			order++;
		}
	}

	m_player.Play();
	m_player.Cleanup();
}