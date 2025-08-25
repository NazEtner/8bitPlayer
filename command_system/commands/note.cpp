#include "note.hpp"
#include <numbers>
#include <cmath>

using CommandSystem::Commands::Note;

void Note::Execute(GroupStatus& status) {
	auto freq = status.noteFrequency;
	auto ticks = status.noteTicks;
	auto bpm = status.bpm;
	auto ticksPerBeat = status.ticksPerBeat;
	double volume = status.volume;
	auto sampleRate = status.sampleRate;
	auto waveform = status.waveform;
	auto decayPerTick = status.decayPerTick;
	double decayPerSample = 0.0;
	if (decayPerTick > 0) {
		// 1ティックあたりのサンプル数を計算
		double samplesPerTick = (60.0 / bpm) * (sampleRate / ticksPerBeat);
		if (samplesPerTick > 0) {
			decayPerSample = static_cast<double>(decayPerTick) / samplesPerTick;
		}
	}
	size_t totalSamples = static_cast<size_t>((60.0 / bpm) * ticks * (sampleRate / ticksPerBeat));
	std::vector<uint8_t> samples(totalSamples, 127);
	if (freq > 0 && volume > 0) {
		switch (waveform) {
		case GroupStatus::eWaveform::Square:
			for (size_t i = 0; i < totalSamples; ++i) {
				double dutyCycle = status.dutyCycle;
				double t = static_cast<double>(i) / sampleRate;
				double waveValue = (fmod(t * freq, 1.0) < dutyCycle) ? 1.0 : -1.0;
				samples[i] = static_cast<uint8_t>(127.5 + waveValue * (volume / 2.0));
				if (volume > decayPerSample) {
					volume -= decayPerSample;
				}
				else {
					volume = 0;
				}
			}
			break;
		case GroupStatus::eWaveform::Triangle:
			for (size_t i = 0; i < totalSamples; ++i) {
				double t = static_cast<double>(i) / sampleRate;
				double waveValue = 2.0 * fabs(2.0 * (t * freq - floor(t * freq + 0.5))) - 1.0;
				samples[i] = static_cast<uint8_t>(127.5 + waveValue * (volume / 2.0));
				if (volume > decayPerSample) {
					volume -= decayPerSample;
				}
				else {
					volume = 0;
				}
			}
			break;
		case GroupStatus::eWaveform::Sine:
			for (size_t i = 0; i < totalSamples; ++i) {
				double t = static_cast<double>(i) / sampleRate;
				double waveValue = sin(2.0 * std::numbers::pi * freq * t);
				samples[i] = static_cast<uint8_t>(127.5 + waveValue * (volume / 2.0));
				if (volume > decayPerSample) {
					volume -= decayPerSample;
				}
				else {
					volume = 0;
				}
			}
			break;
		case GroupStatus::eWaveform::Noise:
			size_t step_count = 0;
			size_t samples_per_step = sampleRate / freq; // freqを反映
			double waveValue = ((rand() % 2000) - 1000) / 1000.0;
			for (size_t i = 0; i < totalSamples; ++i) {
				if (i % samples_per_step == 0) {
					waveValue = ((rand() % 2000) - 1000) / 1000.0; // 新しいランダム値に切り替え
				}
				samples[i] = static_cast<uint8_t>(127.5 + waveValue * (volume / 2.0));
				if (volume > decayPerSample) {
					volume -= decayPerSample;
				}
				else {
					volume = 0;
				}
			}
			break;
		}
	}

	status.soundBuffer.insert(status.soundBuffer.end(), samples.begin(), samples.end());
}