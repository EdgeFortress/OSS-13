#pragma once

#include "Shared/Types.hpp"

class Player;

struct PlayerCommand {
    enum class Code : char {
        NONE = 0,
        JOIN,
		MOVE,
		BUILD,
		GHOST
    };

    virtual const Code GetCode() const final;

protected:
	explicit PlayerCommand(Code code);
private:
	Code code;
};

struct JoinPlayerCommand : public PlayerCommand {
	JoinPlayerCommand();
};

struct MovePlayerCommand : public PlayerCommand {
	uf::vec2i order;
	MovePlayerCommand(uf::vec2i order);
};

struct BuildPlayerCommand : public PlayerCommand {
	BuildPlayerCommand();
};

struct GhostPlayerCommand : public PlayerCommand {
	GhostPlayerCommand();
};