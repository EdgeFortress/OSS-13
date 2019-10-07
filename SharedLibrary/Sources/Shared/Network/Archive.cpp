#include "Archive.h"

#include "ISerializable.h"

#include <Shared/ErrorHandling.h>

using namespace uf;

Archive::Archive() :
	mode(Mode::Input),
	packet(std::make_unique<sf::Packet>())
{ }

Archive::Archive(std::unique_ptr<sf::Packet> packet) :
	mode(Mode::Output),
	packet(std::move(packet))
{ }

uptr<ISerializable> Archive::UnpackSerializable() {
	EXPECT(mode == Mode::Output);

	sf::Int32 id{};
	serializeSimple(id);

	auto serializable = CreateSerializableById(id);
	serializable->Serialize(*this);

	return serializable;
}

Archive::Mode Archive::GetMode() const { return mode; }
void Archive::SetMode(Archive::Mode mode) { this->mode = mode; }

sf::Packet &Archive::GetPacket() { return *packet; }
