#include "WindowSink.h"

#include <Player.hpp>
#include <Shared/Command.hpp>

WindowSink::WindowSink(Player *player, const std::string &layout) :
	player(player),
	layout(layout)
{
	player->AddCommandToClient(new OpenWindowServerCommand(layout));
}
