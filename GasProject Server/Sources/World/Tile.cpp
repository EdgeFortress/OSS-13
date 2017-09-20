#include "Tile.hpp"

#include "Server.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Objects.hpp"
#include "Objects/Control.hpp"
#include "Network/Differences.hpp"

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y),
    locale(nullptr), gases(int(Gas::Count), 0),
    hasFloor(false), fullBlocked(false), directionsBlocked(4, false)
{
    const uint ux = uint(x);
    const uint uy = uint(y);
    sprite = Global::Sprite(unsigned(Global::Sprite::Space) + ((ux + uy) ^ ~(ux * uy)) % 25);

    totalPressure = 0;
}

void Tile::Update() {
    /*for (auto iter = content.begin(); iter != content.end();) {
    auto obj = iter->get();
    if (obj)
    obj->Update();
    if (iter->get() == nullptr)
    iter = content.erase(iter);
    else
    iter++;
    }*/
}

void Tile::CheckLocale() {
    //if (locale || !hasFloor || fullBlocked) return;

    //for (int dx = -1; dx <= 1; dx++)
    //    for (int dy = -1; dy <= 1; dy++) {
    //        if (!map->GetTile(x + dx, y + dy) ||
    //            dx == 0 && dy == 0)
    //            continue;
    //        if (map->GetTile(x + dx, y + dy)->locale) {
    //            if (locale) {
    //                locale->Merge(map->GetTile(x + dx, y + dy)->locale);
    //            } else {
    //                map->GetTile(x + dx, y + dy)->locale->AddTile(this);
    //                locale = map->GetTile(x + dx, y + dy)->locale;
    //            }
    //        }
    //    }
    //if (!locale) {
    //    map->NewLocale(this);
    //}
}

bool Tile::RemoveObject(Object *obj) {
    for (auto iter = content.begin(); iter != content.end(); iter++) {
        if (*iter == obj) {
            if (dynamic_cast<Floor *>(obj)) hasFloor = false;
            else if (dynamic_cast<Wall *>(obj)) fullBlocked = false;
            obj->tile = nullptr;
            content.erase(iter);
            GetBlock()->AddDiff(new RemoveDiff(obj));
            return true;
        }
    }
    return false;
}

void Tile::MoveTo(Object *obj) {
    if (!obj) return;

    if (!obj->IsMovable()) {
        Server::log << "Warning! Try to move immovable object" << std::endl;
        return;
    }

    bool available = true;
    if (obj->GetDensity())
        for (auto &object : content)
            if (object)
                if (object->GetDensity()) {
                    available = false;
                    break;
                }

    if (available) {
        Block *lastBlock = obj->GetTile()->GetBlock();
        const int dx = X() - obj->GetTile()->X();
        const int dy = Y() - obj->GetTile()->Y();
        if (abs(dx) > 1 || abs(dy) > 1)
            Server::log << "Warning! Moving more than a one tile. (Tile::MoveTo)" << std::endl;
        const uf::Direction direction = uf::VectToDirection(sf::Vector2i(dx, dy));
        addObject(obj);
        GetBlock()->AddDiff(new MoveDiff(obj, direction, obj->GetComponent<Control>()->GetSpeed(), lastBlock));
        //if (obj->GetComponent<Control>()->GetPlayer())
        //	Server::log << x << y << std::endl;
    }
}

void Tile::PlaceTo(Object *obj) {
    Tile *lastTile = obj->GetTile();
    Block *lastBlock = nullptr;
    if (lastTile) lastBlock = lastTile->GetBlock();

    // If obj is wall or floor - remove previous and change status
    if (dynamic_cast<Floor *>(obj)) {
        if (hasFloor) {
            for (auto iter = content.begin(); iter != content.end(); iter++) {
                if (dynamic_cast<Floor *>(*iter)) {
                    content.erase(iter);
                    break;
                }
            }
        }
        hasFloor = true;
    } else if (dynamic_cast<Wall *>(obj)) {
        if (!hasFloor) {
            Server::log << "Warning! Try to place wall without floor" << std::endl;
            return;
        }
        if (fullBlocked) {
            for (auto iter = content.begin(); iter != content.end(); iter++) {
                if (dynamic_cast<Wall *>(*iter)) {
                    content.erase(iter);
                    break;
                }
            }
            
        }
        fullBlocked = true;
    }

    addObject(obj);
    GetBlock()->AddDiff(new ReplaceDiff(obj, X(), Y(), lastBlock));
}

Block *Tile::GetBlock() const {
    return map->GetBlock(x / Global::BLOCK_SIZE, y / Global::BLOCK_SIZE);
}

bool Tile::IsDense() const {
    for (auto &obj : content)
        if (obj->GetDensity()) return true;
    return false;
}

const TileInfo Tile::GetTileInfo(uint visibility) const {
    std::list<ObjectInfo> content;
    for (auto &obj : this->content) {
        if (obj->CheckVisibility(visibility))
            content.push_back(obj->GetObjectInfo());
    }
    return std::move(TileInfo(int(sprite), content));
}

void Tile::addObject(Object *obj) {
    Tile *lastTile = obj->GetTile();

    if (lastTile) {
        for (auto iter = lastTile->content.begin(); iter != lastTile->content.end(); iter++)
            if (*iter == obj) {
                lastTile->content.erase(iter);
                break;
            }
    }

    // Count position in tile content by layer
    auto iter = content.begin();
    while (iter != content.end() && (*iter)->GetLayer() < obj->GetLayer())
        iter++;
    content.insert(iter, obj);

    obj->tile = this;
}