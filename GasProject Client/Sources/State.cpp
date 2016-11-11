#include "Client.hpp"
#include "Graphics/Window.hpp"

#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/AuthUI.hpp"
#include "Graphics/UI/GameListUI.hpp"

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