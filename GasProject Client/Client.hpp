#pragma once
#include "Window.hpp"
#include "State.hpp"
#include <string>

class Player {
private:
	std::string pkey;
};

class ClientController {
private:
	/* Unique_ptr will delete it contents in the destruction. So we don't need destructor.
	But unique_ptr can be just one for it content. So we need to use links. See Get-functions. */
	uptr<Player> player;
	uptr<Window> window;
	uptr<State> state;

public:
	/* Work of Client processing in his constructor.
	If error will accident, unique_ptr's will clear memory automatically.
	Such system allow as awake just 1 function of Client from main. */
	ClientController() : player(new Player), 
		                 window(new Window(this)),
						 state(new MenuLoginState(this)){
        Network::Connect("localhost", PORT);
		sf::Clock clock;

		while (window->isOpen()) {
			sf::Time timeElapsed = clock.restart();
			window->Update(timeElapsed);
		}
	}

	ClientController(const ClientController &) = delete;
	ClientController &operator=(const ClientController &) = delete;
	virtual ~ClientController() = default;

	Player *GetClient() { return player.get(); }
	Window *GetWindow() { return window.get(); }
	State *GetState() { return state.get(); }
};