#pragma once

class Player;

struct PlayerCommand {
    enum class Code : char {
        NONE = 0,
        JOIN,
		MOVE
    };

    virtual const Code GetCode() const = 0;
};

struct JoinPlayerCommand : public PlayerCommand {
    virtual const Code GetCode() const override { return Code::JOIN; }
};

struct MovePlayerCommand : public PlayerCommand {
	sf::Vector2i order;
	MovePlayerCommand(sf::Vector2i(order)) : 
		order(order) { }
	virtual const Code GetCode() const override { return Code::MOVE; }
};