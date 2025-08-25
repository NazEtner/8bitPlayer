#pragma once
#include "../command.hpp"
#include <string>

namespace CommandSystem::Commands {
	class Note : public Command {
	public:
		Note() = default;
		virtual ~Note() = default;
		virtual void Execute(GroupStatus& status) override;
	private:
	};
}