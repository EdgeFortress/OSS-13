#pragma once

class ClientController;

/*  Just states, nothing expecial. Each state must have unique realization of functions.
	ClientContoller can change states and use this functions. 
	State has pointer to ClientController for Access main objects, such as Window. */

class State {
private:
	ClientController *clientController;

public:
	State(ClientController *clientController) : clientController(clientController) { }

	virtual void DrawUI() const = 0;
	virtual void DrawTileGrid() const = 0;

	State(const State &) = delete;
	State &operator=(const State &) = delete;
	virtual ~State() = default;
};

class MenuLoginState : public State {
public:
	MenuLoginState(ClientController *clientController) : State(clientController) { }

	virtual void DrawUI() const override;
	virtual void DrawTileGrid() const override;
	
	MenuLoginState(const MenuLoginState &) = delete;
	MenuLoginState &operator=(const MenuLoginState &) = delete;
	virtual ~MenuLoginState() = default;
};

class MenuServerListState : public State {
public:
	MenuServerListState(ClientController *clientController) : State(clientController) { }

	virtual void DrawUI() const override;
	virtual void DrawTileGrid() const override;

	MenuServerListState(const MenuServerListState &) = delete;
	MenuServerListState &operator=(const MenuServerListState &) = delete;
	virtual ~MenuServerListState() = default;
};

class GameLobbyState : public State {
public:
	GameLobbyState(ClientController *clientController) : State(clientController) { }

	virtual void DrawUI() const override;
	virtual void DrawTileGrid() const override;

	GameLobbyState(const GameLobbyState &) = delete;
	GameLobbyState &operator=(const GameLobbyState &) = delete;
	virtual ~GameLobbyState() = default;
};

class GameProcessState : public State {
public:
	GameProcessState(ClientController *clientController) : State(clientController) { }

	virtual void DrawUI() const override;
	virtual void DrawTileGrid() const override;

	GameProcessState(const GameProcessState &) = delete;
	GameProcessState &operator=(const GameProcessState &) = delete;
	virtual ~GameProcessState() = default;
};