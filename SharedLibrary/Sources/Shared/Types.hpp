#pragma once

#include <memory>

typedef unsigned uint;

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
using sptr = std::shared_ptr<T>;

template<class T>
using wptr = std::weak_ptr<T>;

#include "Geometry/Vec2.hpp"
#include "Geometry/Vec3.hpp"
#include "Geometry/Direction.hpp"