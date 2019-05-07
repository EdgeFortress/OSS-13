#pragma once

#include <string>

#include <Shared/Network/ISerializable.h>

struct OverlayInfo : public ISerializable {
	std::string text;
};
