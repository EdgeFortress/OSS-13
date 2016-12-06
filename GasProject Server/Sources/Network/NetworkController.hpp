#pragma once

#include <thread>

#include <SFML/Network.hpp>

#include "Server.hpp"
#include "Player.hpp"
#include "Common/Useful.hpp"

class NetworkController {
private:
    const sf::Time TIMEOUT = sf::microseconds(100);

    bool active;

    uptr<std::thread> thread;

    void working();
    void parsePacket(sf::Packet &, Player *player);

public:
    NetworkController();

    void Start();
    void Stop();
};

sf::Packet &operator<<(sf::Packet &, ServerCommand *);
sf::Packet &operator<<(sf::Packet &, const Diff &);
sf::Packet &operator<<(sf::Packet &, const BlockInfo &);
sf::Packet &operator<<(sf::Packet &, const TileInfo &);
sf::Packet &operator<<(sf::Packet &, const ObjectInfo &);