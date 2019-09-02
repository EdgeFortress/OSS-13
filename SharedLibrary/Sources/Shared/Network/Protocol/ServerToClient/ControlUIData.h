#pragma once

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>
#include <Shared/Network/ArchiveConverters.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(ControlUIData, uf::ISerializable)
	std::string elementId;
	uf::vec2i position;
	std::vector<uint> spritesIds;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);

		ar & elementId;
		ar & position;
		ar & spritesIds;
	}
DEFINE_SERIALIZABLE_END

} // namespace network
} // namespace protocol
