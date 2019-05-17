#pragma once

#include <string>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

namespace network {
namespace protocol {

struct UIData : public uf::ISerializable {
	std::string window;
	std::string handle;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & window;
		ar & handle;
	}
};

DEFINE_SERIALIZABLE(RadioButtonUIData, UIData)
	uint8_t data;

	void Serialize(uf::Archive &ar) override {
		UIData::Serialize(ar);
		ar & data;
	}
};

} // namespace protocol
} // namespace network
