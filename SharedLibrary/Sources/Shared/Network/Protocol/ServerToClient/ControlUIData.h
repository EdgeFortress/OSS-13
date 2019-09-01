#pragma once

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>
#include <Shared/Network/ArchiveConverters.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(ControlUIData, uf::ISerializable)
	uf::vec2i position;
	std::vector<std::string> icons;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);

		ar & position;
		ar & icons;
	}
DEFINE_SERIALIZABLE_END

} // namespace network
} // namespace protocol
