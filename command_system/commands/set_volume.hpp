#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetVolume : public Command {
	public:
		SetVolume(uint8_t ticks) : m_volume(ticks) {}
		virtual ~SetVolume() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		uint8_t m_volume;
	};
}