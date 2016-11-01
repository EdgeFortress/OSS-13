#include "TileGrid.hpp"

Tile::Tile(Block *block) : block(block) {
	sprite = nullptr;
};

void TileGrid::Draw(sf::RenderWindow *window)
{
	Tile *firstTile = this->GetTile(1, 1);
	firstTile->Draw(window);



	/*Tile * firstTile = this->GetTile(1, 1);
	firstTile -> Draw(window);*/

}

TileGrid::TileGrid(list<uptr<Texture>> &textures) :
	textures(textures),
	blocks(NumOfBlocks, vector<Block *>(NumOfBlocks))
{
	for (auto &vect : blocks)
		for (auto &block : vect)
			block = new Block(this);


}

void TileGrid::Initialize()
{

	Tile *firstTile = this->GetTile(1, 1);
	firstTile->SetSprite(1, 0, 0);
	Object * obj = new Object();
	obj->SetSprite(1, 0, 1, 0);
	firstTile->AddObject(obj);
}

void Tile::Draw(sf::RenderWindow *window)
{
	this->GetSprite()->Draw(window, 100, 100);
	int i = 0;
	for (auto &object : content) {
		object.get()->GetSprite()->Draw(window, 100, 100);
	}
}

Tile* Block::GetTile(int x, int y) {
	if (x >= 0 && x < tileGrid->BlockSize && y >= 0 && y < tileGrid->BlockSize) 
		return tiles[y][x];
	CC::log << "Can't return tile " << x << ", " << y << endl;
	return nullptr;
}