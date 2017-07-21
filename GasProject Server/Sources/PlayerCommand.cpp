#include "PlayerCommand.hpp"

const PlayerCommand::Code PlayerCommand::GetCode() const {
	return code;
}

PlayerCommand::PlayerCommand(PlayerCommand::Code code) : code(code) { }

JoinPlayerCommand::JoinPlayerCommand() : PlayerCommand(Code::JOIN) { }

MovePlayerCommand::MovePlayerCommand(uf::vec2i order) :
	PlayerCommand(Code::MOVE),
	order(order) { }