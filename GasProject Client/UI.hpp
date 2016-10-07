#pragma once

#include "State.hpp"

class UI {
public:
	friend void MenuLoginState::DrawUI() const;
	friend void MenuServerListState::DrawUI() const;
	friend void GameLobbyState::DrawUI() const;
	friend void GameProcessState::DrawUI() const;
};