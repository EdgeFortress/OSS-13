#pragma once

#include<SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "TileGrid.hpp"

class TileGrid;

/*  Just states, nothing expecial. Each state must have unique realization of functions.
	ClientContoller can change states and use this functions. 
	State has pointer to ClientController for Access main objects, such as Window. */

class State {
public:
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const = 0;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const = 0;

	State() = default;
	State(const State &) = delete;
	State &operator=(const State &) = delete;
	virtual ~State() = default;
};

class MenuLoginState : public State {
public:
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const override;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const override;
	
	MenuLoginState() = default;
	MenuLoginState(const MenuLoginState &) = delete;
	MenuLoginState &operator=(const MenuLoginState &) = delete;
	virtual ~MenuLoginState() = default;
};

class MenuLoginWaitingState : public State {
private:
    bool loginWaiting;
    bool regWaiting;

public:
    MenuLoginWaitingState(bool loginWaiting, bool regWaiting) :
		loginWaiting(loginWaiting),
		regWaiting(regWaiting) { }

    virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const override;
    virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const override;

	MenuLoginWaitingState() = default;
    MenuLoginWaitingState(const MenuLoginWaitingState &) = delete;
    MenuLoginWaitingState &operator=(const MenuLoginWaitingState &) = delete;
    virtual ~MenuLoginWaitingState() = default;
};

class MenuServerListState : public State {
public:
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const override;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const override;

	MenuServerListState() = default;
	MenuServerListState(const MenuServerListState &) = delete;
	MenuServerListState &operator=(const MenuServerListState &) = delete;
	virtual ~MenuServerListState() = default;
};

class GameLobbyState : public State {
public:
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const override;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const override;

	GameLobbyState() = default;
	GameLobbyState(const GameLobbyState &) = delete;
	GameLobbyState &operator=(const GameLobbyState &) = delete;
	virtual ~GameLobbyState() = default;
};

class GameProcessState : public State {
public:
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const override;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const override;

	GameProcessState() = default;
	GameProcessState(const GameProcessState &) = delete;
	GameProcessState &operator=(const GameProcessState &) = delete;
	virtual ~GameProcessState() = default;
};