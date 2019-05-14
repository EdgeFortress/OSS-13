#pragma once

#include <any>
#include <string>

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>

struct UIData : public uf::ISerializable {
	std::string window;
	std::string handle;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);
		ar & window;
		ar & handle;
	}
};

struct RadioButtonUIData : public UIData {
	DEFINE_SERID("RadioButtonUIData"_crc32)

	uint8_t data;

	void Serialize(uf::Archive &ar) override {
		UIData::Serialize(ar);
		ar & data;
	}
};
