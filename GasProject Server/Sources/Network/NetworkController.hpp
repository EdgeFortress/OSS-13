#pragma once

#include <thread>
#include <chrono>

#include <SFML/Network.hpp>

#include <Shared/Types.hpp>

struct Connection;

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
