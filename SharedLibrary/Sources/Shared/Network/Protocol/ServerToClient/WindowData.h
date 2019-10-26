#pragma once

#include <Shared/Network/ISerializable.h>
#include <Shared/Network/Archive.h>
#include <Shared/Network/Protocol/InputData.h>

namespace network {
namespace protocol {

DEFINE_SERIALIZABLE(WindowData, uf::ISerializable)
	std::vector<uptr<UIData>> fields;

	void Serialize(uf::Archive &ar) override {
		uf::ISerializable::Serialize(ar);

		if (ar.GetMode() == uf::Archive::Mode::Output) {
			sf::Int32 size;
			ar >> size;
			for (int i = 0; i < size; i++)
				fields.push_back(uptr<UIData>(dynamic_cast<UIData *>(ar.UnpackSerializable().release())));
		} else {
			ar << static_cast<sf::Int32>(fields.size());
			for (auto &field : fields)
				ar << *field;
		}
	}
DEFINE_SERIALIZABLE_END

} // namespace protocol
} // namespace network
