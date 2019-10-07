#pragma once

#include <any>
#include <SFML/Network/Packet.hpp>

#include <Shared/Types.hpp>
#include <Shared/ErrorHandling.h>
#include <Shared/IFaces/ICopyable.h>
#include <Shared/Network/ISerializable.h>

namespace uf {

class Archive : public ICopyable {
public:
	Archive();
	Archive(std::unique_ptr<sf::Packet> packet);

	enum class Mode {
		Output,
		Input
	};

	uptr<ISerializable> UnpackSerializable();

	Mode GetMode() const;
	void SetMode(Mode mode);

	sf::Packet &GetPacket();

	template<class T>
	Archive &operator&(T &ser) {
		if constexpr (std::is_base_of_v<ISerializable, T>) {
			sf::Int32 id; *this >> id; // we know type, so drop excess id
			reinterpret_cast<uf::ISerializable&>(ser).Serialize(*this);
		} else {
			serializeSimple(ser);
		}

		return *this;
	}

	template<class T>
	Archive &operator>>(T &ser) {
		if (mode == Mode::Output) *this & ser;
		return *this;
	}

	template<class T>
	Archive &operator<<(const T &ser) {
		if (mode == Mode::Input) *this & const_cast<T&>(ser);
		return *this;
	}

protected:
	template<class T>
	void serializeSimple(T &ser) {
		if (mode == Mode::Output)
			*packet >> ser;
		else
			*packet << ser;
	}

	template<class T>
	void serializeSimple(uptr<T> &ser) {
		if (mode == Mode::Output)
			ser.reset(dynamic_cast<T *>(this->UnpackSerializable().release()));
		else
			reinterpret_cast<uf::ISerializable *>(ser.get())->Serialize(*this);
	}

	template<class T>
	void serializeSimple(sptr<T> &ser) {
		if (mode == Mode::Output)
			ser.reset(dynamic_cast<T *>(this->UnpackSerializable().release()));
		else
			reinterpret_cast<uf::ISerializable *>(ser.get())->Serialize(*this);
	}

	template<>
	void serializeSimple<Archive>(Archive &ser) {
		auto lastSetMode = ser.GetMode();
		if (mode == Mode::Output) {
			ser.SetMode(Mode::Input);
			sf::Int32 size;
			*this >> size;
			while (size--) {
				sf::Int8 byte;
				*this >> byte;
				ser << byte;
			}
		} else {
			ser.SetMode(Mode::Output);
			size_t size = ser.packet->getDataSize();
			*this << sf::Int32(size);
			auto *byte = reinterpret_cast<const sf::Int8 *>(ser.packet->getData());
			for (size_t i = 0; i < size; i++, byte++) {
				*this << *byte;
			}
		}
		ser.SetMode(lastSetMode);
	}

protected:
	Mode mode;

private:
	std::unique_ptr<sf::Packet> packet;
};

}
