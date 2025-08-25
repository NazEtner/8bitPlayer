#include "set_pitch.hpp"

using CommandSystem::Commands::SetPitch;

void SetPitch::Execute(GroupStatus& status) {
	double frequency = 440.0; // A4の周波数
	if (m_pitch.length() < 2 || m_pitch.length() > 3) {
		// 無効なピッチ形式
		return;
	}

	char note = m_pitch[0];
	int octave = m_pitch.length() == 3 ? m_pitch[2] - '0' : m_pitch[1] - '0';
	int semitoneOffset = 0;
	switch (note) {
	case 'C': semitoneOffset = -9; break;
	case 'D': semitoneOffset = -7; break;
	case 'E': semitoneOffset = -5; break;
	case 'F': semitoneOffset = -4; break;
	case 'G': semitoneOffset = -2; break;
	case 'A': semitoneOffset = 0; break;
	case 'B': semitoneOffset = 2; break;
	default:
		// 無効なノート
		return;
	}
	if (m_pitch.length() == 3) {
		char accidental = m_pitch[1];
		if (accidental == '#') {
			semitoneOffset += 1;
		}
		else if (accidental == 'b') {
			semitoneOffset -= 1;
		}
		else {
			// 無効な臨時記号
			return;
		}
	}

	int semitonesFromA4 = semitoneOffset + (octave - 4) * 12;
	frequency = 440.0 * pow(2.0, semitonesFromA4 / 12.0);
	status.noteFrequency = frequency;
}