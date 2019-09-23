#pragma once

#include <vector>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>
#include <Shared/Network/ArchiveConverters.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(ContextMenuNode, uf::ISerializable)
	std::string title;
	std::vector<std::string> verbs;

	void Serialize(uf::Archive &archive) {
		uf::ISerializable::Serialize(archive);
		archive & title;
		archive & verbs;
	}
DEFINE_SERIALIZABLE_END

DEFINE_SERIALIZABLE(ContextMenuData, uf::ISerializable)
	std::vector<std::unique_ptr<ContextMenuNode>> nodes;

	void Serialize(uf::Archive &archive) {
		uf::ISerializable::Serialize(archive);
		archive & nodes;
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
