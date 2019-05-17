#pragma once

#include <string>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

namespace network {
namespace protocol {

struct OverlayInfo : public uf::ISerializable {
	DEFINE_SERID("OverlayInfo"_crc32)

	std::string text;

	void Serialize(uf::Archive &archive) {
		uf::ISerializable::Serialize(archive);
		archive & text;
	}
};

} // namespace protocol
} // namespace network
