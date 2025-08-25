#pragma once
#include "group_status.hpp"

namespace CommandSystem {
	class Command {
	public:
		Command() = default;
		virtual ~Command() = default;
		virtual void Execute(GroupStatus& status) = 0;
	};
}