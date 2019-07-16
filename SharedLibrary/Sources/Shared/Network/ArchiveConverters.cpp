#include "ArchiveConverters.h"

uf::Archive &operator&(uf::Archive &ar, uf::Direction &d) {
	if (ar.IsOutput()) {
		sf::Int8 buf;
		ar >> buf;
		d = static_cast<uf::Direction>(buf);
	} else {
		ar << static_cast<sf::Int8>(d);
	}
	return ar;
}
