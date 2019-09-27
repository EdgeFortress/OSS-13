#pragma once

#include <Shared/ThreadSafeQueue.hpp>
#include <Shared/IFaces/INonCopyable.h>
#include <Shared/Network/Protocol/Command.h>

namespace network {

class CommandsProcessor : public INonCopyable {
public:
	void AddCommand(uptr<network::protocol::Command> &&command) {
		syncCommands.Push(std::forward<uptr<network::protocol::Command>>(command));
	}

	void ProcessCommands() {
		while (!syncCommands.Empty()) {
			auto generalCommand = syncCommands.Pop();
			ProcessCommand(*generalCommand);
		}
	}

protected:
	virtual void ProcessCommand(network::protocol::Command &generalCommand) = 0;

private:
	uf::ThreadSafeQueue<uptr<network::protocol::Command>> syncCommands;
};

} // namespace network

#define _REGISTRATE_COMMAND_PROCESSOR(commandType)                      \
	if (auto *command = dynamic_cast<commandType *>(&generalCommand)) { \
		commandProcessor_##commandType(*command);                       \
		return;                                                         \
	}
