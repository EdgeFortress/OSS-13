#include "MovePhysics.hpp"

#include "Shared/Math.hpp"

namespace uf {
    namespace phys {

        uf::vec2f countDeltaShift(sf::Time timeElapsed, uf::vec2f shift, float moveSpeed, uf::vec2i moveIntent, uf::vec2f constSpeed, uf::vec2f physSpeed) {
            uf::vec2f deltaShift;

            if (constSpeed) {
                deltaShift += constSpeed * timeElapsed.asSeconds();
                return deltaShift;
            } 
            
            if (moveSpeed) {
                deltaShift += moveIntent * moveSpeed * timeElapsed.asSeconds();
                
                //// Aligning by tile boundaries if no intention to moving
                shift += deltaShift;
                if (shift) {
                    float delta = timeElapsed.asSeconds() * moveSpeed;

                    // X
                    if (!deltaShift.x && shift.x) {
                        if (shift.x * uf::sgn(shift.x) > delta)
                            deltaShift.x -= uf::sgn(shift.x) * delta;
                        else deltaShift.x -= shift.x;
                    }
                    // Y
                    if (!deltaShift.y && shift.y) {
                        if (shift.y * uf::sgn(shift.y) > delta)
                            deltaShift.y -= uf::sgn(shift.y) * delta;
                        else deltaShift.y -= shift.y;
                    }
                }

                return deltaShift;
            }

            return deltaShift;
        }
    }
}