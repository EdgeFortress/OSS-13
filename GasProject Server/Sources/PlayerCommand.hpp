#pragma once

class Player;

struct PlayerCommand {
    enum class Code : char {
        NONE = 0,
        JOIN
    };

    virtual const Code GetCode() const = 0;
};

struct JoinPlayerCommand : public PlayerCommand {
    virtual const Code GetCode() const override { return Code::JOIN; }
};