#include "set_waveform.hpp"
#include <iostream>

using CommandSystem::Commands::SetWaveform;

void SetWaveform::Execute(GroupStatus& status) {
	if (m_waveform == "square") {
		status.waveform = GroupStatus::eWaveform::Square;
	}
	else if (m_waveform == "triangle") {
		status.waveform = GroupStatus::eWaveform::Triangle;
	}
	else if (m_waveform == "sine") {
		status.waveform = GroupStatus::eWaveform::Sine;
	}
	else if (m_waveform == "noise") {
		status.waveform = GroupStatus::eWaveform::Noise;
	}
	else {
		// –³Œø‚È”gŒ`Žw’è
		std::cout << "Invalid waveform: " << m_waveform << std::endl;
	}
}