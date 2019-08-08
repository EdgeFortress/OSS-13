#include "Archive.h"

#include "ISerializable.h"

#include <Shared/ErrorHandling.h>

using namespace uf;

// Archive

Archive::Archive(sf::Packet &packet) :
	packet(packet)
{ }

#define DECLARE_SER(name) \
	case #name##_crc32: { uptr<ISerializable> p = std::make_unique<name>(); p->Serialize(*this); return p; }

uptr<ISerializable> Archive::UnpackSerializable() {
	sf::Int32 id;
	packet >> id;

	auto serializable = CreateSerializableById(id);
	serializable->Serialize(*this);

	return serializable;
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
