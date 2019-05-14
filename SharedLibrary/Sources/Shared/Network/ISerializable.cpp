#include "ISerializable.h"

#include "Archive.h"

namespace uf {

void ISerializable::Serialize(Archive &archive) {
	archive << sf::Int32(Id());
}

} // namespace uf
