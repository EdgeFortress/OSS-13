#include "AtmosOverlayWindowSink.h"

#include <Server.hpp>

AtmosOverlayWindowSink::AtmosOverlayWindowSink(Player *player) :
	WindowSink(player, "AtmosOverlayWindow.json")
{ }

void AtmosOverlayWindowSink::OnTrigger(const std::string &trigger) {

}

void AtmosOverlayWindowSink::OnInput(const std::string &input, uptr<UIData> &&data) {
	Server::log << "Input trigged: window '" << data->window << "', handle '" << data->handle << "'" << std::endl;
}
