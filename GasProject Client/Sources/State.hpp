#pragma once

#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "Graphics/TileGrid/TileGrid.hpp"

class TileGrid;

/*  Just states, nothing expecial. Each state must have unique realization of functions.
	ClientContoller can change states and use this functions. 
	State has pointer to ClientController for Access main objects, such as Window. */

class State {
public:
    virtual void Initialize() const = 0;
    virtual void Ending() const = 0;
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const = 0;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const = 0;

	State() = default;
	State(const State &) = delete;
	State &operator=(const State &) = delete;
	virtual ~State() = default;
};

class MenuLoginState : public State {
public:
    virtual void Initialize() const final;
    virtual void Ending() const final;
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const final;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const final;
	
	MenuLoginState() = default;
	MenuLoginState(const MenuLoginState &) = delete;
	MenuLoginState &operator=(const MenuLoginState &) = delete;
	virtual ~MenuLoginState() = default;
};

class MenuGameListState : public State {
public:
    virtual void Initialize() const final;
    virtual void Ending() const final;
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const final;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const final;

	MenuGameListState() = default;
	MenuGameListState(const MenuGameListState &) = delete;
	MenuGameListState &operator=(const MenuGameListState &) = delete;
	virtual ~MenuGameListState() = default;
};

class GameLobbyState : public State {
public:
    virtual void Initialize() const final;
    virtual void Ending() const final;
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const final;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const final;

	GameLobbyState() = default;
	GameLobbyState(const GameLobbyState &) = delete;
	GameLobbyState &operator=(const GameLobbyState &) = delete;
	virtual ~GameLobbyState() = default;
};

class GameProcessState : public State {
public:
    virtual void Initialize() const final;
    virtual void Ending() const final;
	virtual void DrawUI(sf::RenderWindow *window, sf::Time timeElapsed) const final;
	virtual void DrawTileGrid(sf::RenderWindow *render_window, TileGrid *tileGrid) const final;

	GameProcessState() = default;
	GameProcessState(const GameProcessState &) = delete;
	GameProcessState &operator=(const GameProcessState &) = delete;
	virtual ~GameProcessState() = default;
};