#pragma once

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

namespace network {
namespace protocol {

struct Command : public uf::ISerializable {
	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
	}
};

} // namespace protocol
} // namespace network
