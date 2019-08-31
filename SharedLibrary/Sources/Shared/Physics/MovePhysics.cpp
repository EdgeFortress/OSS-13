#include "MovePhysics.hpp"

#include "Shared/Math.hpp"

namespace uf {
    namespace phys {

        uf::vec2f countDeltaShift(sf::Time timeElapsed, uf::vec2f shift, float moveSpeed, uf::vec2i moveIntent, uf::vec2f speed) {
            uf::vec2f deltaShift;

            if (speed) {
                deltaShift += speed * timeElapsed.asSeconds();
                return deltaShift;
            } 
            
            if (moveSpeed) {
                // Speed shouldn't be greater when moving diagonally
                if (moveIntent.x != 0 && moveIntent.y != 0)
                    moveSpeed /= 1.414213f; // sqrt(2)

                deltaShift += moveIntent * moveSpeed * timeElapsed.asSeconds();

                shift += deltaShift;
                //// Stop moving if shift > moveIntent
                if (moveIntent) {
                    if (moveIntent.x && uf::abs(shift.x) > uf::abs(moveIntent.x) && shift.x * moveIntent.x > 0)
                        deltaShift.x += moveIntent.x - shift.x;
                    if (moveIntent.y && uf::abs(shift.y) > uf::abs(moveIntent.y) && shift.y * moveIntent.y > 0)
                        deltaShift.y += moveIntent.y - shift.y;
                }

                //// Aligning by tile boundaries if no intention to moving
                if (shift) {
                    float delta = timeElapsed.asSeconds() * moveSpeed;

                    // X
                    if (!moveIntent.x && deltaShift.x == 0 && shift.x != 0) {
                        if (shift.x * uf::sgn(shift.x) > delta)
                            deltaShift.x -= uf::sgn(shift.x) * delta;
                        else deltaShift.x -= shift.x;
                    }
                    // Y
                    if (!moveIntent.y && deltaShift.y == 0 && shift.y != 0) {
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