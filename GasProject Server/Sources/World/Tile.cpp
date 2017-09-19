#include "Tile.hpp"

#include "Server.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Objects.hpp"
#include "Objects/Control.hpp"
#include "Network/Differences.hpp"

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y)//, local(nullptr)
{
    const uint ux = uint(x);
    const uint uy = uint(y);
    sprite = Global::Sprite(unsigned(Global::Sprite::Space) + ((ux + uy) ^ ~(ux * uy)) % 25);
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

    int position = 0;
    auto iter = content.begin();
    while (iter != content.end() && (*iter)->layer < obj->layer)
        iter++, position++;
    content.insert(iter, obj);

    obj->tile = this;
}

bool Tile::RemoveObject(Object *obj) {
    for (auto &i : content)
        if (i == obj) {
            //i.release();
            i = nullptr;
            content.remove(i);
            obj->tile = nullptr;
            //add local remove
            GetBlock()->AddDiff(new RemoveDiff(obj));
            return true;
        }
    return false;
}

void Tile::MoveTo(Object *obj) {
    if (!obj) return;

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
    addObject(obj);
    GetBlock()->AddDiff(new ReplaceDiff(obj, X(), Y(), lastBlock));
}

Block *Tile::GetBlock() const {
    return map->GetBlock(x / Global::BLOCK_SIZE, y / Global::BLOCK_SIZE);
}

//void Tile::CheckLocal() {
//    if (local) return;
//
//    bool floor = false;
//    for (auto &obj : content) {
//        if (dynamic_cast<Wall *>(obj)) {
//            return;
//        }
//        if (dynamic_cast<Floor *>(obj)) {
//            floor = true;
//        }
//    }
//    if (!floor) return;
//
//    for (int dx = -1; dx <= 1; dx++)
//        for (int dy = -1; dy <= 1; dy++) {
//            if (!map->GetTile(x + dx, y + dy) ||
//                dx == 0 && dy == 0)
//                continue;
//            if (map->GetTile(x + dx, y + dy)->local) {
//                if (local) {
//                    local->Merge(map->GetTile(x + dx, y + dy)->local);
//                } else {
//                    map->GetTile(x + dx, y + dy)->local->AddTile(this);
//                    local = map->GetTile(x + dx, y + dy)->local;
//                }
//            }
//        }
//    if (!local) {
//        map->NewLocal(this);
//    }
//}

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

bool Tile::IsDense() {
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