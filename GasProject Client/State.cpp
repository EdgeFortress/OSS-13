#include "Client.hpp"
#include "Window.hpp"
#include "UI.hpp"
#include "State.hpp"

void MenuLoginState::Initialize() const { 
    CC::Get()->GetWindow()->GetUI()->GetAuthUI()->Show();
}

void MenuGameListState::Initialize() const { 
    CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Show();
}

void GameLobbyState::Initialize() const { }
void GameProcessState::Initialize() const { }

void MenuLoginState::Ending() const {  
    CC::Get()->GetWindow()->GetUI()->GetAuthUI()->Hide();
}

void MenuGameListState::Ending() const { 
    CC::Get()->GetWindow()->GetUI()->GetGameListUI()->Hide();
}

void GameLobbyState::Ending() const { }
void GameProcessState::Ending() const { }