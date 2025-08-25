#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class SetPitch : public Command {
	public:
		SetPitch(const std::string& pitch) : m_pitch(pitch) {}
		virtual ~SetPitch() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
		std::string m_pitch;
	};
}