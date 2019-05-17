#include "WelcomeWindowSink.h"

#include <Server.hpp>

WelcomeWindowSink::WelcomeWindowSink(Player *player) :
	WindowSink(player, "DynamicWindow.json") 
{ }

void WelcomeWindowSink::OnTrigger(const std::string &trigger) {

}

void WelcomeWindowSink::OnInput(const std::string &input, uptr<network::protocol::UIData> &&data) {
	Server::log << "Input trigged: window '" << data->window << "', handle '" << data->handle << "'" << std::endl;
}
