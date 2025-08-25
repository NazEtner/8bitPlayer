#pragma once
#include <xaudio2.h>
#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>

namespace Audio {
	class Player {
	public:
		Player();
		virtual ~Player();
		void AddBuffer(const uint32_t order, std::vector<uint8_t>& buffer, const uint32_t sampleRate); // orderÇ™ìØÇ∂Ç‡ÇÃÇÕìØéûÇ…çƒê∂
		void Play();
		void NotifyPlaybackEnd(const uint32_t order);
		void Cleanup();
	private:
		IXAudio2* m_xaudio = nullptr;
		IXAudio2MasteringVoice* m_master = nullptr;
		std::map<uint32_t, std::vector<IXAudio2SourceVoice*>> m_voices;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::map<uint32_t, size_t> m_finished_voices;
		std::vector<std::vector<uint8_t>> m_audio_copy;
	};

	class VoiceCallback : public IXAudio2VoiceCallback {
	public:
		VoiceCallback(Player* player, uint32_t order) : m_player(player), m_order(order) {}

		void STDMETHODCALLTYPE OnStreamEnd() override;

		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) override {}
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {}
		void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override {}
		void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override {}
		void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override {}
		void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) override {}
	private:
		Player* m_player;
		uint32_t m_order;
	};
}