#pragma once

#include "Shared/Types.hpp"

class Player;

struct PlayerCommand {
    enum class Code : char {
        NONE = 0,
        JOIN,
		MOVE,
		MOVEZ,
        CLICK_OBJECT,
		CLICK_CONTROL_UI,
		VERB
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

struct MoveZPlayerCommand : public PlayerCommand {
	bool order;
	MoveZPlayerCommand(bool order);
};

struct ClickObjectPlayerCommand : public PlayerCommand {
    uint id;
    ClickObjectPlayerCommand(uint id);
};

struct ClickControlUIPlayerCommand : public PlayerCommand {
	std::string id;
	ClickControlUIPlayerCommand(const std::string &);
};

struct VerbPlayerCommand : public PlayerCommand {
	std::string verb;
	VerbPlayerCommand(const std::string &verb);
};
