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

	virtual uptr<UIData> Clone() const = 0;
};

DEFINE_SERIALIZABLE(RadioButtonUIData, UIData)
	int data;

	void Serialize(uf::Archive &ar) override {
		UIData::Serialize(ar);
		ar & data;
	}

	uptr<UIData> Clone() const override { 
		return std::make_unique<RadioButtonUIData>(*this);
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
