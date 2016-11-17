#pragma once

//#include "Player.hpp"

class Player;

struct NetworkCommand {
    enum class Code : char {
        ADD_PLAYER = 1
    };

    virtual const Code GetCode() const = 0;
};

struct AddPlayerCommand : public NetworkCommand {
    Player *player;

    virtual const Code GetCode() const override { return Code::ADD_PLAYER; }

    AddPlayerCommand(Player *player) : player(player) { }
};