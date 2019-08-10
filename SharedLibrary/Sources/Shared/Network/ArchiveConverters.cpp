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

uf::Archive &operator&(uf::Archive &ar, uf::DirectionSet &directionSet) {
	if (ar.IsOutput()) {
		sf::Int8 buf;
		ar >> buf;
		directionSet.SetBuffer(buf);
	} else {
		ar << static_cast<sf::Int8>(directionSet.GetBuffer());
	}
	return ar;
}

uf::Archive &operator&(uf::Archive &ar, uf::DirectionSetFractional &directionSetFractional) {
	if (ar.IsOutput()) {
		std::array<float, 5> buf;
		ar & buf;
		directionSetFractional.SetFractions(std::move(buf));
	} else {
		ar & directionSetFractional.GetFractions();
	}
	return ar;
}
