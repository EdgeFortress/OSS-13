#include <vector>

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "TileGrid.hpp"
#include "Common/NetworkConst.hpp"

Object::Object(const Global::Sprite key, const bool directed) {
    //if (directed) direction = 0;
    //else direction = -1;
    direction = 0;
    SetSprite(key);
}

void Object::SetSprite(const Global::Sprite key) {
    sprite = nullptr;
    if (int(key))
        for (auto &sprite : CC::Get()->GetWindow()->GetSprites())
            if (sprite->GetKey() == key) {
                this->sprite = sprite.get();
                break;
            }
};

void Object::Draw(sf::RenderWindow * const window, const int x, const int y) {
    if (sprite) sprite->Draw(window, x, y, direction);
}

Tile::Tile(Block *block, const int x, const int y) : 
    block(block), x(x), y(y),
    sprite(nullptr) 
{
};

void Tile::Draw(sf::RenderWindow * const window, const int x, const int y) const {
    //int sizeTile = CC::Get()->GetWindow()->GetSizeTile();
    //int xPosition = sizeTile * xNum;
    //int yPosition = sizeTile * yNum;

    //Sprite* tileSprite = this->GetSprite();
    //if (tileSprite) {
    //    tileSprite->SetSize(sizeTile);
    //    tileSprite->Draw(window, xPosition, yPosition);
    //}
   
    sprite->Draw(window, x, y, 0);

    for (auto &obj : content)
        obj->Draw(window, x, y);
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

TileGrid::TileGrid(const int windowWidth, const int windowHeight) : blockSize(Global::BLOCK_SIZE) {
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    int num_of_visible_blocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    num_of_visible_blocks = num_of_visible_blocks / Global::BLOCK_SIZE + (num_of_visible_blocks % Global::BLOCK_SIZE ? 1 : 0);

    blocks.resize(num_of_visible_blocks);

    for (auto &vect : blocks) {
        vect.resize(num_of_visible_blocks);
        for (auto &block : vect)
            block.reset(new Block(this));
    }
}

void TileGrid::Draw(sf::RenderWindow * const window) {
    Lock();
    for (int y = -(Global::FOV / 2); y <= Global::FOV / 2; y++)
        for (int x = -(Global::FOV / 2); x <= Global::FOV / 2; x++) {
            Tile *tile = GetTile(xPos + x, yPos + y);
            if (tile) tile->Draw(window, xPadding + (x + Global::FOV / 2) * tileSize,
                                         yPadding + (y + Global::FOV / 2) * tileSize);
        }
    Unlock();
}

void TileGrid::Resize(const int windowWidth, const int windowHeight) {
    tileSize = min(windowWidth, windowHeight) / Global::FOV;
    xNumOfTiles = 15;
    yNumOfTiles = 15;
    xPadding = (windowWidth - tileSize * xNumOfTiles) / 2;
    yPadding = (windowHeight - tileSize * yNumOfTiles) / 2;
}

void TileGrid::Move(int blockID, int x, int y, int objectNum, int toX, int toY) {
    Block *block = GetBlock(blockID);
    if (!block) {
        CC::log << "Wrong BlockID accepted: " << blockID << endl;
        return;
    }
    Tile *tile = block->GetTile(x, y);
    Tile *new_tile = block->GetTile(toX, toY);
    if (!tile) {
        CC::log << "Wrong block (" << blockID << ") coordinates: " << x << y << endl;
        return;
    }
    if (!new_tile) {
        CC::log << "Wrong block (" << blockID << ") coordinates: " << x << y << endl;
        return;
    }
    auto obj = tile->RemoveObject(objectNum);
    new_tile->AddObject(obj.release());
}

Block *TileGrid::GetBlock(int blockID) const {
    for (auto &vect : blocks) {
        for (auto &block : vect) {
            if (block->GetID() == blockID) {
                return block.get();
            }
        }
    }
    return nullptr;
}

Tile *TileGrid::GetTile(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize)
        return blocks[y / blockSize][x / blockSize]->GetTile(x % blockSize, y % blockSize);
    //CC::log << "Can't return block " << x << ", " << y << endl;
    return nullptr;
}

Tile * TileGrid::GetTileByPixel(int x, int y) const
{
    int xTile = (x - xPadding) / tileSize + xPos - xNumOfTiles / 2;
    int yTile = (y - yPadding) / tileSize + yPos - yNumOfTiles / 2;

    return GetTile(xTile, yTile);
}
