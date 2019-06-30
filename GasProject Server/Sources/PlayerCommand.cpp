#include "PlayerCommand.hpp"

const PlayerCommand::Code PlayerCommand::GetCode() const {
	return code;
}

PlayerCommand::PlayerCommand(PlayerCommand::Code code) : code(code) { }

JoinPlayerCommand::JoinPlayerCommand() : 
	PlayerCommand(Code::JOIN) { }

MovePlayerCommand::MovePlayerCommand(uf::vec2i order) :
	PlayerCommand(Code::MOVE),
	order(order) { }

MoveZPlayerCommand::MoveZPlayerCommand(bool order) :
	PlayerCommand(Code::MOVEZ),
	order(order) { }

ClickObjectPlayerCommand::ClickObjectPlayerCommand(uint id) :
    PlayerCommand(Code::CLICK_OBJECT),
    id(id) { }

DropPlayerCommand::DropPlayerCommand() :
	PlayerCommand(Code::DROP)
{ }

BuildPlayerCommand::BuildPlayerCommand() :
	PlayerCommand(Code::BUILD) { }

GhostPlayerCommand::GhostPlayerCommand() :
	PlayerCommand(Code::GHOST) { }

VerbPlayerCommand::VerbPlayerCommand(const std::string &verb) :
	PlayerCommand(Code::VERB),
	verb(verb)
{ }
