#include <vector>

#include <SFML/Window/Event.hpp>

#include "TileGrid.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Common/NetworkConst.hpp"
#include "Network.hpp"

Object::Object(const Global::Sprite key, const string name, const bool directed) {
    SetSprite(key);
    this->name = name;
    direction = 0;
}

void Object::SetSprite(const Global::Sprite key) {
    sprite = nullptr;
    if (int(key))
        for (auto &sprite : CC::Get()->GetWindow()->GetSprites())
            if (sprite->GetKey() == key) {
                this->sprite = sprite.get();
                return;
            }
}
bool Object::checkObj(int x, int y) {
    return !(sprite->PixelTransparent(x, y));
}

void Object::Draw(sf::RenderWindow * const window, const int x, const int y) {
    if (sprite) sprite->Draw(window, x + int(shift.x), y + int(shift.y), direction);
}

void Object::Update(sf::Time timeElapsed) {
    //shift += timeElapsed.asSeconds() * shiftingSpeed * CC::Get()->GetWindow()->GetTileGrid()->GetTileSize() * sf::Vector2f(shiftingDirection);
}

Tile::Tile(Block *block, const int x, const int y) : 
    block(block), x(x), y(y),
    sprite(nullptr) 
{
};

void Tile::Draw(sf::RenderWindow * const window, const int x, const int y) const {
    if (sprite) sprite->Draw(window, x, y, 0);

    //for (auto &obj : content)
    //    obj->Draw(window, x, y);
}

int Tile::GetRelX() const { return block->GetRelX() * block->GetTileGrid()->GetBlockSize() + x; }
int Tile::GetRelY() const { return block->GetRelY() * block->GetTileGrid()->GetBlockSize() + y; }

Object *Tile::GetObjectByCoord(const unsigned x, const unsigned y) const {
    for (auto obj = content.rbegin(); obj != content.rend(); obj++) {
        if ((*obj)->checkObj(x, y))
            return *obj;
    }
    return nullptr;
}

Block::Block(TileGrid *tileGrid) : 
    tileGrid(tileGrid), relX(relX), relY(relY),
    tiles(tileGrid->GetBlockSize())
{
    int y = 0;
    for (auto &vect : tiles) {
        vect.resize(tileGrid->GetBlockSize());
        int x = 0;
        for (auto &tile : vect) {
            tile.reset(new Tile(this, x, y));
            x++;
        }
        y++;
    }
}

Tile* Block::GetTile(int x, int y) const {
    if (x >= 0 && x < tileGrid->GetBlockSize() && y >= 0 && y < tileGrid->GetBlockSize())
        return tiles[y][x].get();
    return nullptr;
}

TileGrid::TileGrid() : 
    blockSize(Global::BLOCK_SIZE),
    firstBlockX(0), firstBlockY(0)
{
    //
    // Count num of visible blocks
    //
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    numOfVisibleBlocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    numOfVisibleBlocks = numOfVisibleBlocks / Global::BLOCK_SIZE + (numOfVisibleBlocks % Global::BLOCK_SIZE ? 1 : 0);

    blocks.resize(numOfVisibleBlocks);

    for (auto &vect : blocks) {
        vect.resize(numOfVisibleBlocks);
        //for (auto &block : vect)
        //    block.reset(new Block(this));
    }
}

void TileGrid::Draw(sf::RenderWindow * const window) {
    mutex.lock();
    std::multimap<int, Object *> objects;
    for (int y = -(Global::FOV / 2); y <= Global::FOV / 2; y++)
        for (int x = -(Global::FOV / 2); x <= Global::FOV / 2; x++) {
            Tile *tile = GetTileRel(xRelPos + x, yRelPos + y);
            if (tile) {
                tile->Draw(window, xPadding + (x + Global::FOV / 2) * tileSize,
                                   yPadding + (y + Global::FOV / 2) * tileSize);
                for (auto &obj : tile->GetContent())
                    objects.insert(std::pair<int, Object *>(obj->GetLayer(), obj));
            }
        }
    //CC::log << "!" << objects.size() << endl;
    for (auto &pair : objects)
        pair.second->Draw(window, xPadding + (pair.second->GetTile()->GetRelX() - xRelPos + Global::FOV / 2) * tileSize,
                                  yPadding + (pair.second->GetTile()->GetRelY() - yRelPos + Global::FOV / 2) * tileSize);
    mutex.unlock();
}

void TileGrid::Resize(const int windowWidth, const int windowHeight) {
    tileSize = min(windowWidth, windowHeight) / Global::FOV;
    xNumOfTiles = 15;
    yNumOfTiles = 15;
    xPadding = 0;
    yPadding = (windowHeight - tileSize * yNumOfTiles) / 2;
}

void TileGrid::HandleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                moveCommand.y = -1;
                //CC::log << moveCommand.y << endl;
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                moveCommand.y = 1;
                //CC::log << moveCommand.y << endl;
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                moveCommand.x = 1;
                //CC::log << moveCommand.x << endl;
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                moveCommand.x = -1;
                //CC::log << moveCommand.x << endl;
                break;
            default:
                break;
        }
    }
}

void TileGrid::Update(sf::Time timeElapsed) {
    if (moveSendPause != sf::Time::Zero) {
        moveSendPause -= timeElapsed;
        if (moveSendPause < sf::Time::Zero) moveSendPause = sf::Time::Zero;
    }
    if (moveSendPause == sf::Time::Zero) {
        Connection::commandQueue.Push(new MoveClientCommand(Global::VectToDirection(moveCommand)));
        moveSendPause = MOVE_TIMEOUT;
        moveCommand = sf::Vector2i();
    }

    mutex.lock();
    for (auto &iter = objects.begin(); iter != objects.end();) {
        (*iter)->Update(timeElapsed);
        if (!(*iter)->GetTile()) iter = objects.erase(iter);
        else iter++;
    }
    mutex.unlock();
}

void TileGrid::LockDrawing() {
    mutex.lock();
}

void TileGrid::UnlockDrawing() {
    mutex.unlock();
}

void TileGrid::AddObject(Object *object) {
    objects.push_back(uptr<Object>(object));
}

void TileGrid::RemoveObject(uint id) {
    for (auto iter = objects.begin(); iter != objects.end(); iter++)
        if ((*iter)->GetID() == id) {
            Object *obj = iter->get();
            if (obj->GetTile()) obj->GetTile()->RemoveObject(obj);
            objects.erase(iter);
            return;
        }
    CC::log << "Error: object with id" << id << "is not exist" << endl;
}

void TileGrid::MoveObject(uint id, int toX, int toY, int toObjectNum) {
    Tile *tile = GetTileAbs(toX, toY);
    if (!tile) {
        CC::log << "Wrong tile absolute coords (" << toX << toY << ")" << endl;
        return;
    }
    //CC::log << "ID" << id << endl;
    for (auto &obj : objects)
        if (obj->GetID() == id) {
            tile->AddObject(obj.get(), toObjectNum);
            return;
        }
    CC::log << "Wrong object ID:" << id << endl;
}

void TileGrid::ShiftObject(uint id, Global::Direction direction, float speed) {
    for (auto &obj : objects)
        if (obj->GetID() == id) {
            obj->SetShifting(direction, speed);
            //tile->AddObject(obj.get(), toObjectNum);
            return;
        }
}

void TileGrid::ShiftBlocks(const int newFirstX, const int newFirstY) {
    int dx = newFirstX - firstBlockX;
    int dy = newFirstY - firstBlockY;
    
    vector< vector<sptr<Block>> > newBlocks(numOfVisibleBlocks);
    for (auto &vect : newBlocks)
        vect.resize(numOfVisibleBlocks);

    for (int y = 0; y < numOfVisibleBlocks; y++) {
        for (int x = 0; x < numOfVisibleBlocks; x++) {
            if (x - dx >= 0 && x - dx < numOfVisibleBlocks &&
                y - dy >= 0 && y - dy < numOfVisibleBlocks) {
                newBlocks[y - dy][x - dx] = blocks[y][x];
                newBlocks[y - dy][x - dx]->relX = x - dx;
                newBlocks[y - dy][x - dx]->relY = y - dy;
            }
        }
    }

    blocks = std::move(newBlocks);

    firstBlockX = newFirstX;
    firstBlockY = newFirstY;

    firstTileX = firstBlockX * blockSize;
    firstTileY = firstBlockY * blockSize;
}

void TileGrid::SetCameraPosition(const int x, const int y) {
    xPos = x;
    yPos = y;
    xRelPos = x - firstTileX;
    yRelPos = y - firstTileY;
}

void TileGrid::SetBlock(int x, int y, Block *block) {
    blocks[y - firstBlockY][x - firstBlockX].reset(block);
    block->relX = x - firstBlockX;
    block->relY = y - firstBlockY;
    //if (!neededBlocks) UnlockDrawing();
}

wptr<Block> TileGrid::GetBlock(const int blockX, const int blockY) const {
    int relBlockX = blockX - firstBlockX;
    int relBlockY = blockY - firstBlockY;

    if (relBlockX < 0 || relBlockX >= numOfVisibleBlocks ||
        relBlockY < 0 || relBlockY >= numOfVisibleBlocks)
        return wptr<Block>();

    return wptr<Block>(blocks[relBlockY][relBlockX]);
}

Tile *TileGrid::GetTileRel(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize) {
        Block *block = blocks[y / blockSize][x / blockSize].get();
        if (block)
            return block->GetTile(x % blockSize, y % blockSize);
        else
            return nullptr;
    }
    return nullptr;
}

Tile *TileGrid::GetTileAbs(int x, int y) const {
    x -= firstTileX;
    y -= firstTileY;
    return GetTileRel(x, y);
}

Object *TileGrid::GetObjectByPixel(int x, int y) const {
    int xTile = (x - xPadding) / tileSize + xRelPos - xNumOfTiles / 2;
    int yTile = (y - yPadding) / tileSize + yRelPos - yNumOfTiles / 2;
    Tile *locTile = GetTileRel(xTile, yTile);
    if (locTile == nullptr) return nullptr;

    int xLoc = (x - xPadding) % tileSize;
    int yLoc = (y - yPadding) % tileSize;
    Object *locObj = locTile->GetObjectByCoord(xLoc, yLoc);

    return locObj;
}