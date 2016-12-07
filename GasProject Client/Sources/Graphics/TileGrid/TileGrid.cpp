#include <vector>

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "TileGrid.hpp"
#include "Common/NetworkConst.hpp"

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
    if (sprite) sprite->Draw(window, x, y, direction);
}

Tile::Tile(Block *block, const int x, const int y) : 
    block(block), x(x), y(y),
    sprite(nullptr) 
{
};

void Tile::Draw(sf::RenderWindow * const window, const int x, const int y) const {
    if (sprite) sprite->Draw(window, x, y, 0);

    for (auto &obj : content)
        obj->Draw(window, x, y);
}

Object *Tile::GetObjectByCoord(const unsigned x, const unsigned y) const {
    for (auto obj = content.rbegin(); obj != content.rend(); obj++) {
        if ((*obj)->checkObj(x, y))
            return (*obj).get();
    }
    return nullptr;
}

Block::Block(TileGrid *tileGrid) : 
    tileGrid(tileGrid), id(-1),
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

Tile* Block::GetTile (int x, int y) const {
    if (x >= 0 && x < tileGrid->GetBlockSize() && y >= 0 && y < tileGrid->GetBlockSize())
        return tiles[y][x].get();
    //CC::log << "Can't return tile " << x << ", " << y << endl;
    return nullptr;
}

TileGrid::TileGrid() : 
    blockSize(Global::BLOCK_SIZE),
    firstBlockX(0), firstBlockY(0),
    drawingLocked(false)
{
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    numOfVisibleBlocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    numOfVisibleBlocks = numOfVisibleBlocks / Global::BLOCK_SIZE + (numOfVisibleBlocks % Global::BLOCK_SIZE ? 1 : 0);

    blocks.resize(numOfVisibleBlocks);

    for (auto &vect : blocks) {
        vect.resize(numOfVisibleBlocks);
        for (auto &block : vect)
            block.reset(new Block(this));
    }
}

void TileGrid::Draw(sf::RenderWindow * const window) {
    mutex.lock();
    for (int y = -(Global::FOV / 2); y <= Global::FOV / 2; y++)
        for (int x = -(Global::FOV / 2); x <= Global::FOV / 2; x++) {
            Tile *tile = GetTile(xRelPos + x, yRelPos + y);
            if (tile) tile->Draw(window, xPadding + (x + Global::FOV / 2) * tileSize,
                                         yPadding + (y + Global::FOV / 2) * tileSize);
        }
    mutex.unlock();
}

void TileGrid::Resize(const int windowWidth, const int windowHeight) {
    tileSize = min(windowWidth, windowHeight) / Global::FOV;
    xNumOfTiles = 15;
    yNumOfTiles = 15;
    xPadding = 0;
    yPadding = (windowHeight - tileSize * yNumOfTiles) / 2;
}

void TileGrid::LockDrawing() {
    mutex.lock();
}

void TileGrid::UnlockDrawing() {
    mutex.unlock();
}

void TileGrid::Move(int blockX, int blockY, int x, int y, int objectNum, int toX, int toY, int toObjectNum) {
    sptr<Block> block = GetBlock(blockX, blockY).lock();
    if (!block) {
        CC::log << "Wrong BlockID accepted: " << blockX << blockY << endl;
        return;
    }
    Tile *tile = block->GetTile(x, y);
    Tile *new_tile = block->GetTile(toX, toY);
    if (!tile) {
        CC::log << "Wrong block (" << blockX << blockY << ") coordinates: " << x << y << endl;
        return;
    }
    if (!new_tile) {
        CC::log << "Wrong block (" << blockX << blockY << ") coordinates: " << x << y << endl;
        return;
    }
    auto obj = tile->RemoveObject(objectNum);
    new_tile->AddObject(obj.release(), toObjectNum);
}

void TileGrid::Add(int blockX, int blockY, int x, int y, int objectNum, Global::Sprite sprite, string name) {
    sptr<Block> block = GetBlock(blockX, blockY).lock();
    if (!block) {
        CC::log << "Wrong BlockID accepted: " << blockX << blockY << endl;
        return;
    }
    Tile *tile = block->GetTile(x, y);
    if (!tile) {
        CC::log << "Wrong block (" << blockX << blockY << ") coordinates: " << x << y << endl;
        return;
    }

    tile->AddObject(new Object(sprite, name), objectNum);
}

void TileGrid::Remove(int blockX, int blockY, int x, int y, int objectNum) {
    sptr<Block> block = GetBlock(blockX, blockY).lock();
    if (!block) {
        CC::log << "Wrong BlockID accepted: " << blockX << blockY << endl;
        return;
    }
    Tile *tile = block->GetTile(x, y);
    if (!tile) {
        CC::log << "Wrong block (" << blockX << blockY << ") coordinates: " << x << y << endl;
        return;
    }

    tile->RemoveObject(objectNum);
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
                y - dy >= 0 && y - dy < numOfVisibleBlocks)
                newBlocks[y - dy][x - dx] = blocks[y][x];
        }
    }

    blocks = std::move(newBlocks);

    firstBlockX = newFirstX;
    firstBlockY = newFirstY;
}

void TileGrid::SetCameraPosition(const int x, const int y) {
    xPos = x;
    yPos = y;
    xRelPos = x - firstBlockX * Global::BLOCK_SIZE;
    yRelPos = y - firstBlockY * Global::BLOCK_SIZE;
}

void TileGrid::SetBlock(int x, int y, Block *block) {
    blocks[y - firstBlockY][x - firstBlockX].reset(block);
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

Tile *TileGrid::GetTile(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize) {
        Block *block = blocks[y / blockSize][x / blockSize].get();
        if (block)
            return block->GetTile(x % blockSize, y % blockSize);
        else
            return nullptr;
    }
    //CC::log << "Can't return block" << x << "," << y << endl;
    return nullptr;
}

Object *TileGrid::GetObjectByPixel(int x, int y) const {
    int xTile = (x - xPadding) / tileSize + xRelPos - xNumOfTiles / 2;
    int yTile = (y - yPadding) / tileSize + yRelPos - yNumOfTiles / 2;
    Tile *locTile = GetTile(xTile, yTile);
    if (locTile == nullptr) return nullptr;
    int xLoc = (x - xPadding) % tileSize;
    int yLoc = (y - yPadding) % tileSize;
    Object *locObj = locTile->GetObjectByCoord(xLoc, yLoc);

    return locObj;
}