#include <iostream>
#include <list>
#include <mutex>

#include "SFML/System.hpp"

#include "Server.hpp"
#include "Network/NetworkController.hpp"
#include "Common/Ñommand.hpp"
#include "World/World.hpp"
#include "Player.hpp"
#include "Database/UsersDB.hpp"

using namespace std;
using namespace sf;

Game::Game(string title, int id) : title(title),
                                   id(id),
                                   active(true),
                                   thread(new std::thread(&Game::gameProcess, this)) {

}

void Game::gameProcess() {
    CurThreadGame = this;
    world.reset(new World());
    world->FillingWorld();
    Clock clock;
    while (active) {
        update(clock.restart());
        sleep(seconds(0.1f));
    }
}

void Game::update(sf::Time timeElapsed) {
    world->Update(timeElapsed);
    {
        std::unique_lock<std::mutex> lock(playersLock);

        for (auto iter = players.begin(); iter != players.end();) {
            if (sptr<Player> player = iter->lock()) {
                player->Update();
                iter++;
            } else
                iter = players.erase(iter);
        }

        for (wptr<Player> player : players)
            if (sptr<Player> player_s = player.lock())
                player_s->SendGraphicsUpdates();
    }
}

const int Game::GetID() const { return id; }

bool Game::AddPlayer(wptr<Player> player) {
    std::unique_lock<std::mutex> lock(playersLock);
    players.push_back(player);
    return true;
}

//void Game::DeletePlayer(Player *player) { 
//    std::unique_lock<std::mutex> lock(playersLock);
//    players.remove(player); 
//}

void Game::SendChatMessage(const std::vector<std::wstring> &message, Player *player) {
    //std::unique_lock<std::mutex> lock(playersLock);
    //for (auto &player : players) {
    //    if (iter != player)
    //        iter->AddCommandToClient(new SendChatMessageServerCommand(message, player->GetCKey()));
    //}
}

Game::~Game() {
    active = false;
    thread->join();
}

Server::Server() : UDB(new UsersDB()),
                   networkController(new NetworkController()),
                   new_game_id(1) {
    instance = this;
    networkController->Start();
    CreateGame("One Super Test Game");
    while (true) {
        {
            std::unique_lock<std::mutex> lock(playersLock);
            for (auto iter = players.begin(); iter != players.end(); ) {
                sptr<Player> player = *iter;
                if (!(player->IsConnected())) {
                    iter = players.erase(iter);
                } else
                    iter++;
            }
        }
        sleep(seconds(1));
    }
}

bool Server::Authorization(string &login, string &password) const {
    if (UDB->Check(login, password)) {
        bool isNew = true;
        for (auto &i : players)
            if (i->GetCKey() == login) {
                isNew = false;
                break;
            }
        if (isNew) {
        Server::log << "Player is authorized:" << login << password << endl;
        return true;
    }
        Server::log << "Player" << login << password << "is trying to authorize second time" <<endl;
        return false;
    }
    Server::log << "Wrong login data received:" << login << password << endl;
    return false;
}

bool Server::Registration(string &login, string &password) const {
    if (UDB->Add(login, password)) {
        Server::log << "New player is registrated:" << login << password << endl;
        return true;
    }
    Server::log << "Player is trying make second accoun with same login:" << login << password << endl;
    return false;
}

bool Server::CreateGame(string title) {
    games.push_back(uptr<Game>(new Game(title, new_game_id)));
    new_game_id++;
    return true;
}

const std::list<uptr<Game>> * const Server::GetGamesList() const {
    return &games;
}

Game *Server::JoinGame(const int id, Player *raw_player) const {
    // Cast raw pointer to shared_ptr
    for (auto player : players) {
        if (player.get() == raw_player) {

            for (auto &game : games) {
                if (game->GetID() == id) {
                    if (game->AddPlayer(player)) {
                        return game.get();
                    }
                }
            }

        }
    }
    return nullptr;
}

void Server::AddPlayer(sptr<Player> player) {
    std::unique_lock<std::mutex> lock(playersLock);
    players.push_back(player);
}

int main() {
    Server server;

    return 0;
}

Log Server::log;
Server *Server::instance;
thread_local Game *CurThreadGame = nullptr;