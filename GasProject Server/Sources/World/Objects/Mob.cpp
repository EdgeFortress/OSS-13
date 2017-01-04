#include "Mob.hpp"

#include "Server.hpp"
#include "World/World.hpp"
#include "Network/Differences.hpp"

void Mob::Move(sf::Vector2i order) {
    if (!order.x && !order.y) return;
    std::unique_lock<std::mutex> lock(orderLock);
    if (order.x) moveOrder.x = order.x;
    if (order.y) moveOrder.y = order.y;
    tile->GetBlock()->AddDiff(new ShiftDiff(id, Global::VectToDirection(order), speed));
}

void Mob::Update(sf::Time timeElapsed) {
    std::unique_lock<std::mutex> lock(orderLock);
    if (moveOrder.x || moveOrder.y) {
        shift += speed * sf::Vector2f(moveOrder) * (timeElapsed.asMilliseconds() / 1000.f);

        if (shift.x || shift.y) {
            int dx, dy;
            dx = dy = 0;
            if (abs(shift.x) >= 0.5f) {
                dx += int(sgn(shift.x) * floor(abs(shift.x) - 0.5f + 1.f));
                shift.x -= dx;
            }
            if (abs(shift.y) >= 0.5f) {
                dy += int(sgn(shift.y) * floor(abs(shift.y) - 0.5f + 1.f));
                shift.y -= dy;
            }

            if (dx || dy) {
                Tile *dest_tile = tile->GetMap()->GetTile(tile->X() + dx, tile->Y() + dy);
                if (dest_tile)
                    dest_tile->MoveTo(this);

                if (dx) moveOrder.x -= sgn(moveOrder.x);
                if (dy) moveOrder.y -= sgn(moveOrder.y);
            }
        }
    }
    if (shift.x && !moveOrder.x) {
        float newShiftX = shift.x - sgn(shift.x) * speed * (timeElapsed.asMilliseconds() / 1000.f);
        shift.x = sgn(shift.x) * sgn(newShiftX) > 0 ? newShiftX : 0;
    }
    if (shift.y && !moveOrder.y) {
        float newShiftY = shift.y - sgn(shift.y) * speed * (timeElapsed.asMilliseconds() / 1000.f);
        shift.y = sgn(shift.y) * sgn(newShiftY) > 0 ? newShiftY : 0;
    }

    //if (shift.x || shift.y)
    //Server::log << "Shift: " << std::to_string(shift.x) << std::to_string(shift.y) << endl;
}