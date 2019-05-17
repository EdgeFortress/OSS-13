#include "ISerializable.h"

#include "Archive.h"

using namespace uf;

void ISerializable::Serialize(Archive &archive) {
	archive << sf::Int32(Id());
}
