#pragma once

#include <string>

#include "../GasProject Server/net_const.hpp"

#include <iostream>
using std::cout;
using std::endl;

using std::string;

class Window;
class State;

class Player {
private:
	string pkey;
};

class ClientController {
private:
	/* Unique_ptr will delete it contents in the destruction. So we don't need destructor.
	But unique_ptr can be just one for it content. So we need to use links. See Get-functions. */
	uptr<Player> player;
	uptr<Window> window;
	uptr<State> state;
    State *newState;

public:
	/* Work of Client processing in this constructor.
	Such system allow as awake just 1 function of Client from main. */
    ClientController();
    void Run();

	ClientController(const ClientController &) = delete;
	ClientController &operator=(const ClientController &) = delete;
	virtual ~ClientController() = default;

    void SetState(State *state) { newState = state; }

	Player *GetClient() { return player.get(); }
	Window *GetWindow() { return window.get(); }
	State *GetState() { return state.get(); }
};