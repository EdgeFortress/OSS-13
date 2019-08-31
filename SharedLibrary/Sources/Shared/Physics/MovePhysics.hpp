#pragma once

#include <SFML/System.hpp>

#include "Shared/Types.hpp"

namespace uf {
    namespace phys {
        uf::vec2f countDeltaShift(sf::Time timeElapsed, uf::vec2f shift, float moveSpeed, uf::vec2i moveIntent, uf::vec2f speed);
    }
}
