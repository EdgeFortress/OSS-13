#pragma once

#include <Shared/IFaces/IHasRepeatableID.h>
#include <Shared/Geometry/DirectionSet.h>

#include <World/ISubsystem.h>

namespace subsystem {

namespace atmos {

struct ILocale : public IFace, public IHasRepeatableID {

};

} // namespace atmos

struct IAtmos : public ISubsystem {

};

} // namespace subsystem
