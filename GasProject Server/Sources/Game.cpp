#include <Game.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

#include <Shared/Command.hpp>

#include <Network/Connection.hpp>
#include <World/World.hpp>
#include <World/Objects/Control.hpp>
#include <World/Objects/Creature.hpp>

Game::Game() :
	active(true)
{
	thread = std::make_unique<std::thread>(&Game::gameProcess, this);
}

void Game::gameProcess() {
	world.reset(new World());
	world->FillingWorld();
	sf::Clock clock;
	while (active) {
		update(clock.restart());
		sf::Time timeToSleep = sf::seconds(0.05f) - clock.getElapsedTime(); // 10 ticks per second
		if (timeToSleep > sf::Time::Zero)
			sf::sleep(timeToSleep);
	}
}

void Game::update(sf::Time timeElapsed) {
	world->Update(timeElapsed);
	{
		std::unique_lock<std::mutex> lock(playersLock);
		for (auto iter = players.begin(); iter != players.end();) {
			sptr<Player> player = *iter;
			if (player->IsConnected()) {
				player->Update(timeElapsed);
				iter++;
			} else {
				// If player disconnected, move him into disconnectedPlayers list
				player->Suspend();
				if (iter == players.begin()) {
					disconnectedPlayers.splice(disconnectedPlayers.end(), players, iter);
					iter = players.begin();
				} else {
					auto temp = iter;
					temp--;
					disconnectedPlayers.splice(disconnectedPlayers.end(), players, iter);
					iter = temp++;
				}
			}
		}

		for (wptr<Player> player : players)
			if (sptr<Player> player_s = player.lock())
				player_s->SendGraphicsUpdates(timeElapsed);
	}
	SendChatMessages();
}

bool Game::AddPlayer(sptr<Player> &player) {
	std::unique_lock<std::mutex> lock(playersLock);
	for (auto iter = disconnectedPlayers.begin(); iter != disconnectedPlayers.end(); iter++) {
		sptr<Player> &cur_player = *iter;
		if (cur_player->GetCKey() == player->GetCKey()) {
			sptr<Connection> connection = player->GetConnection();
			connection->player = cur_player;
			cur_player->SetConnection(connection);
			players.splice(players.end(), disconnectedPlayers, iter);

			return false;
		}
	}
	players.push_back(player);
	player->JoinToGame();

	return true;
}

Control *Game::GetStartControl(Player *player) {
	return world->CreateNewPlayerCreature()->GetComponent<Control>();
}

void Game::SendChatMessages() {
	std::vector<std::string> messages = chat.GetNewMessages();
	for (auto &player : players)
		for (auto &message : messages)
			player->AddCommandToClient(new SendChatMessageServerCommand(message));
}

Game::~Game() {
	active = false;
	thread->join();
}

IGame *GGame = nullptr;
