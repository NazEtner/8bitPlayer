#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace CommandSystem {
	struct GroupStatus {
		enum class eWaveform {
			Square,
			Triangle,
			Sine,
			Noise
		} waveform = eWaveform::Square;
		std::string groupName;
		std::vector<uint8_t> soundBuffer;
		const size_t sampleRate = 44100;
		uint16_t bpm = 120;
		uint8_t ticksPerBeat = 4;
		uint8_t volume = 255;
		uint16_t noteTicks = 4;
		double noteFrequency = 440; // A4
		double dutyCycle = 0.5; // 50%
		uint16_t decayPerTick = 0;
	};
}