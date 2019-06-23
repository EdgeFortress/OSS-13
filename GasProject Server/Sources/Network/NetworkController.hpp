#pragma once

#include <thread>
#include <chrono>

#include <SFML/Network.hpp>

#include "Shared/Command.hpp"
#include "Shared/TileGrid_Info.hpp"

struct Connection;
struct Diff;

class NetworkController {
private:
	const std::chrono::microseconds TIMEOUT{100};

    bool active;
    uptr<std::thread> thread;
	std::list< sptr<Connection> > connections;

    void working();
	// return false if received "disconnect" packet
    bool parsePacket(sf::Packet &, sptr<Connection> &connection);

public:
    NetworkController();

    void Start();
    void Stop();
};

sf::Packet &operator<<(sf::Packet &, ServerCommand *);
sf::Packet &operator<<(sf::Packet &, const Diff &);
sf::Packet &operator<<(sf::Packet &, const TileInfo &);
sf::Packet &operator<<(sf::Packet &, const ObjectInfo &);