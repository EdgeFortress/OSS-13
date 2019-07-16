#pragma once

#include <any>
#include <SFML/Network/Packet.hpp>

#include <Shared/Types.hpp>
#include <Shared/Network/ISerializable.h>

namespace uf {

class Archive  {
public:
	Archive(sf::Packet &packet);
	virtual ~Archive() = default;

	template<class T>
	Archive &operator&(T &ser) {
		if constexpr (std::is_base_of_v<ISerializable, T>) {
			sf::Int32 id; *this >> id; // we know type, so drop excess id
			reinterpret_cast<uf::ISerializable&>(ser).Serialize(*this);
		} else {
			serialize(ser);
		}

		return *this;
	}

	template<class T>
	Archive &operator>>(T &ser) {
		if (isOut) *this & ser;
		return *this;
	}

	template<class T>
	Archive &operator<<(const T &ser) {
		if (!isOut) *this & const_cast<T&>(ser);
		return *this;
	}

	uptr<ISerializable> UnpackSerializable();
	bool IsOutput() { return isOut; }

protected:
	template<class T>
	void serialize(T &ser) {
		if (isOut)
			packet >> ser;
		else
			packet << ser;
	}

	template<class T>
	void serialize(uptr<T> &ser) {
		if (isOut)
			ser.reset(dynamic_cast<T *>(this->UnpackSerializable().release()));
		else
			reinterpret_cast<uf::ISerializable *>(ser.get())->Serialize(*this);
	}

protected:
	bool isOut;

private:
	sf::Packet &packet;
};

class InputArchive : public Archive {
public:
	explicit InputArchive(sf::Packet &packet);
};

class OutputArchive : public Archive {
public:
	explicit OutputArchive(sf::Packet &packet);
};

}
