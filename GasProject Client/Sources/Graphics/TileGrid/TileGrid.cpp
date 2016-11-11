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

Tile::Tile(Block *block) : block(block), sprite(nullptr) {
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
    for (auto &vect : tiles) {
        vect.resize(tileGrid->GetBlockSize());
        for (auto &tile : vect)
            tile.reset(new Tile(this));
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

Tile *TileGrid::GetTile(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize)
        return blocks[y / blockSize][x / blockSize]->GetTile(x % blockSize, y % blockSize);
    //CC::log << "Can't return block " << x << ", " << y << endl;
    return nullptr;
}

void TileGrid::Draw(sf::RenderWindow * const window) {
    Lock();
    for (int y = -(Global::FOV / 2); y < Global::FOV / 2; y++)
        for (int x = -(Global::FOV / 2); x < Global::FOV / 2; x++) {
            Tile *tile = GetTile(xPos + x, yPos + y);
            if (tile) tile->Draw(window, xPadding + (x + Global::FOV / 2) * tileSize,
                                         yPadding + (y + Global::FOV / 2) * tileSize);
        }
    Unlock();
}

void TileGrid::Resize(const int windowWidth, const int windowHeight) {
    tileSize = min(windowWidth, windowHeight) / Global::FOV;
    xPadding = (windowWidth - tileSize * 15) / 2;
    yPadding = (windowHeight - tileSize * 15) / 2;
}