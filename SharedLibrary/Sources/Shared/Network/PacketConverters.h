#pragma once

#include <SFML/Network/Packet.hpp>

namespace uf {
enum class Direction : char;
}

sf::Packet &operator<<(sf::Packet &packet, uf::Direction &direction);
sf::Packet &operator>>(sf::Packet &packet, uf::Direction &direction);
