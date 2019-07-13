#include <Game.h>

#include <plog/Log.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>

#include <Shared/Command.hpp>

#include <Network/Connection.hpp>
#include <ScriptEngine/ScriptEngine.h>
#include <World/World.hpp>
#include <World/Objects/Control.hpp>
#include <World/Map.hpp>

using namespace std::chrono_literals;

Game::Game() :
	active(true)
{
	thread = std::make_unique<std::thread>(&Game::gameProcess, this);
}

void Game::gameProcess() {
	scriptEngine = std::make_unique<ScriptEngine>();
	world.reset(new World());
	scriptEngine->FillMap(world->GetMap());
	world->CreateTestItems();
	auto lastTime = std::chrono::steady_clock::now();
	while (active) {
		auto curTime = std::chrono::steady_clock::now();
		auto timeElapsed = curTime - lastTime;
		lastTime = curTime;

		try {
			update(std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed));
		} catch (const std::exception &e) {
			LOGE << "Main game cycle failed due exception: " << "\n"
				 << e.what();
			getchar();
		}

		auto timeToSleep = 50us - timeElapsed; // 10 ticks per second
		if (timeToSleep > timeToSleep.zero())
			std::this_thread::sleep_for(timeToSleep);
	}
}

void Game::update(std::chrono::microseconds timeElapsed) {
	DelayedActivitiesManager::Update(timeElapsed);

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
	GetScriptEngine()->OnPlayerJoined(player);
	return dynamic_cast<Control *>(world->CreateNewPlayerCreature()->GetComponent("Control"));
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
