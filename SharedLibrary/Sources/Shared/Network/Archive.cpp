#include "Archive.h"

#include "ISerializable.h"

#include <Shared/Network/Protocol/OverlayInfo.h>
#include <Shared/Network/Protocol/InputData.h>

namespace uf {

// Archive

Archive::Archive(sf::Packet &packet) :
	packet(packet)
{ }

#define DECLARE_SER(name) \
	case #name##_crc32: { uptr<ISerializable> p = std::make_unique<##name>(); p->Serialize(*this); return p; }

uptr<ISerializable> Archive::UnpackSerializable() {
	sf::Int32 id;
	packet >> id;

	switch (id) {
		DECLARE_SER(OverlayInfo)
		DECLARE_SER(RadioButtonUIData)
		default:
			throw std::exception(); // unknown id
	}
}


// InputArchive

InputArchive::InputArchive(sf::Packet &packet) :
	Archive(packet)
{ 
	isOut = false;
}

// OutputArchive

OutputArchive::OutputArchive(sf::Packet &packet) :
	Archive(packet)
{ 
	isOut = true;
}

} // namespace uf
