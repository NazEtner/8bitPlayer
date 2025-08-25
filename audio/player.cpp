#include "player.hpp"

using Audio::Player;
using Audio::VoiceCallback;

void VoiceCallback::OnStreamEnd() {
	// Player クラスに再生完了を通知
	if (m_player) {
		m_player->NotifyPlaybackEnd(m_order);
	}
}

Player::Player() {
	try {
		HRESULT hr = XAudio2Create(&m_xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create XAudio2 engine.");
		}

		hr = m_xaudio->CreateMasteringVoice(&m_master);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create mastering voice.");
		}
	}
	catch (const std::exception) {
		if (m_master) {
			m_master->DestroyVoice();
			m_master = nullptr;
		}
		if (m_xaudio) {
			m_xaudio->Release();
			m_xaudio = nullptr;
		}
		throw;
	}
}

Player::~Player() {
	for (auto& pair : m_voices) {
		for (IXAudio2SourceVoice* voice : pair.second) {
			voice->Stop();
			voice->DestroyVoice();
		}
	}
	if (m_master) {
		m_master->DestroyVoice();
	}
	if (m_xaudio) {
		m_xaudio->Release();
	}
}

void Player::AddBuffer(const uint32_t order, std::vector<uint8_t>& buffer, const uint32_t sampleRate) {
	if (!m_xaudio) {
		throw std::runtime_error("XAudio2 engine is not initialized.");
	}

	WAVEFORMATEX wfx = { 0 };
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = sampleRate;
	wfx.wBitsPerSample = 8;
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize = 0;

	// コールバックオブジェクトの作成
	IXAudio2VoiceCallback* callback = new VoiceCallback(this, order);

	// ソースボイスの作成 (コールバックを指定)
	IXAudio2SourceVoice* sourceVoice = nullptr;
	HRESULT hr = m_xaudio->CreateSourceVoice(&sourceVoice, &wfx, 0, 1.0f, callback);
	if (FAILED(hr)) {
		delete callback;
		throw std::runtime_error("Failed to create source voice with callback.");
	}

	// オーディオバッファの準備
	m_audio_copy.push_back(buffer);
	XAUDIO2_BUFFER xaudioBuffer = { 0 };
	xaudioBuffer.AudioBytes = static_cast<UINT32>(m_audio_copy.back().size());
	xaudioBuffer.pAudioData = m_audio_copy.back().data();
	xaudioBuffer.Flags = XAUDIO2_END_OF_STREAM;

	// バッファをボイスに送信
	hr = sourceVoice->SubmitSourceBuffer(&xaudioBuffer);
	if (FAILED(hr)) {
		sourceVoice->DestroyVoice();
		delete callback;
		throw std::runtime_error("Failed to submit source buffer.");
	}

	// ボイスとコールバックをマップに追加
	m_voices[order].push_back(sourceVoice);
}

void Player::Play() {
	if (m_voices.empty()) {
		return;
	}

	// order の昇順で再生
	for (auto const& [order, voices] : m_voices) {
		// 現在の order のボイスをすべて再生開始
		for (IXAudio2SourceVoice* voice : voices) {
			voice->Start(0);
		}

		// 同時再生が完了するまで待機
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this, order]() {
				// この order のボイスがすべて終了したかチェック
				return m_finished_voices[order] == m_voices[order].size();
			});
		}
	}
}

void Player::NotifyPlaybackEnd(uint32_t order) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_finished_voices[order]++;
	m_cv.notify_one();
}

void Player::Cleanup() {
	for (auto& pair : m_voices) {
		for (IXAudio2SourceVoice* voice : pair.second) {
			voice->Stop();
			voice->DestroyVoice(); // callbackって消えるのか？
		}
	}
	m_voices.clear();
	m_finished_voices.clear();
	m_audio_copy.clear();
}