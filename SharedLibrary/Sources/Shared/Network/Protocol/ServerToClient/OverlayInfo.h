#pragma once

#include <string>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(OverlayInfo, uf::ISerializable)
	std::string text;

	void Serialize(uf::Archive &archive) {
		uf::ISerializable::Serialize(archive);
		archive & text;
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
