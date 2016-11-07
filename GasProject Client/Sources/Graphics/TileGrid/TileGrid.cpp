#include <vector>

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "TileGrid.hpp"
#include "Common/NetworkConst.hpp"

Tile::Tile(Block *block) : block(block), sprite(Global::Sprite::EMPTY) {
};

void Tile::Draw(sf::RenderWindow *window,int xNum = 0, int yNum = 0) {
	/*int sizeTile = CC::Get()->GetWindow()->GetSizeTile();
	int xPosition = sizeTile * xNum;
	int yPosition = sizeTile * yNum ;

	Sprite* tileSprite = this->GetSprite();
	if (tileSprite) {
		tileSprite->SetSize(sizeTile);
		tileSprite->Draw(window, xPosition, yPosition);
	}
	
    for (auto &object : content) {
		Sprite* objSprite = object.get()->GetSprite();
		objSprite->SetSize(sizeTile);
		objSprite->Draw(window, xPosition, yPosition);
    }*/
}

Block::Block(TileGrid *tileGrid) : 
	tileGrid(tileGrid),
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

TileGrid::TileGrid() : blockSize(Global::BLOCK_SIZE) {
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    int num_of_visible_blocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    num_of_visible_blocks = num_of_visible_blocks / Global::BLOCK_SIZE + (num_of_visible_blocks % Global::BLOCK_SIZE ? 1 : 0);

    blocks.resize(num_of_visible_blocks);

    for (auto &vect : blocks)
        for (auto &block : vect)
            block.reset(new Block(this));
}

Tile *TileGrid::GetTile(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize)
        return blocks[y / blockSize][x / blockSize]->GetTile(x % blockSize, y % blockSize);
    //CC::log << "Can't return block " << x << ", " << y << endl;
    return nullptr;
}

void TileGrid::Draw(sf::RenderWindow *window) {
    Lock();
	//for(int i = 0; i < 15; i++)
	//	for (int j = 0; j < 15; j++)
	//		this->GetTile(xPos + i, yPos + j)->Draw(window, i, j);
    Unlock();
}
