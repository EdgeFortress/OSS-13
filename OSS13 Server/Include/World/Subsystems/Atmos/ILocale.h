#pragma once

#include <Shared/IFaces/IFace.h>
#include <Shared/IFaces/IHasRepeatableID.h>

namespace subsystem::atmos {

struct ILocale : public IFace, public IHasRepeatableID {

};

} // namespace subsystem::atmos
