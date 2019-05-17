#include "PacketConverters.h"

#include <Shared/Types.hpp>

sf::Packet &operator<<(sf::Packet &packet, uf::Direction &direction) {
	packet << static_cast<sf::Int8>(direction);
	return packet;
}

sf::Packet &operator>>(sf::Packet &packet, uf::Direction &direction) {
	sf::Int8 i;
	packet >> i;
	direction = static_cast<uf::Direction>(i);
	return packet;
}