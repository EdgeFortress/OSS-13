#include "ArchiveConverters.h"

uf::Archive &operator&(uf::Archive &ar, long int &li) {
    if (ar.IsOutput()) {
        sf::Int32 buf;
        ar >> buf;
        li = buf;
    } else {
        ar << sf::Int32(li);
    }
    return ar;
}

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
