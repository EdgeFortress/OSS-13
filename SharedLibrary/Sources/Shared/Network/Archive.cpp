#include "Archive.h"

#include "ISerializable.h"

namespace uf {

// Archive

template<class T>
void serialize(sf::Packet &packet, T *value, bool isOut) {
	if (isOut)
		packet >> *value;
	else
		packet << *value;
}

Archive &Archive::serialize(std::any &ser, bool isOut) {
	if (auto *pser = std::any_cast<ISerializable *>(&ser))
		(*pser)->Serialize(*this);
	else if (auto *pser = std::any_cast<std::string *>(&ser)) {
		uf::serialize(packet, *pser, isOut);
	}
	else if (auto *pser = std::any_cast<int32_t *>(&ser))
		uf::serialize(packet, *pser, isOut);
	else 
		std::exception(); // TODO: add logging

	return *this;
}

Archive::Archive(sf::Packet &packet) :
	packet(packet)
{ }

Archive &Archive::operator<<(std::any &ser) {
	return serialize(ser, false);
}

Archive &Archive::operator>>(std::any &ser) {
	return serialize(ser, true);
}

// InputArchive

InputArchive::InputArchive(sf::Packet &packet) :
	Archive(packet)
{ }

Archive &InputArchive::pack(std::any ser) {
	return *this << ser;
}

// OutputArchive

OutputArchive::OutputArchive(sf::Packet &packet) :
	Archive(packet)
{ }

Archive &OutputArchive::pack(std::any ser) {
	return *this >> ser;
}

} // namespace uf
