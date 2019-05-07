#pragma once

#include <any>
#include <SFML/Network/Packet.hpp>

namespace uf {

class Archive  {
public:
	Archive(sf::Packet &packet);

	Archive &operator<<(std::any &ser);
	Archive &operator>>(std::any &ser);

	Archive &operator&(std::any &ser) {
		return pack(ser);
	}
	template<class T>
	Archive &operator&(T &ser) {
		return pack(std::any(&ser));
	}

protected:
	virtual Archive &pack(std::any &ser) = 0;

private:
	Archive &serialize(std::any &ser, bool isOut);

private:
	sf::Packet &packet;
};

class InputArchive : public Archive {
public:
	explicit InputArchive(sf::Packet &packet);

protected:
	Archive &pack(std::any &ser) override;
};

class OutputArchive : public Archive {
public:
	explicit OutputArchive(sf::Packet &packet);

protected:
	Archive &pack(std::any &ser) override;
};

}
