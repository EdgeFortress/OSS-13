#include "WelcomeWindowSink.h"

#include <Server.hpp>

WelcomeWindowSink::WelcomeWindowSink(Player *player, const std::string &layout) :
	WindowSink(player, layout) 
{ }

void WelcomeWindowSink::OnTrigger(const std::string &trigger) {

}

void WelcomeWindowSink::OnInput(const std::string &input, uptr<UIData> &&data) {
	Server::log << "Input trigged: window '" << data->window << "', handle '" << data->handle << "'" << std::endl;
}
