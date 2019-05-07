#pragma once

#include <string>

#include <Shared/Network/ISerializable.h>

struct OverlayInfo : public uf::ISerializable {
	std::string text;

	void Serialize(uf::Archive &archive) {
		archive & text;
	}
};
