#include "TileGrid.hpp"

Tile::Tile(Block *block) : block(block) {
	/*HARDCORE ADD SOME OBJECTS*/
	/*Object* obj1 = new Object;
	obj1->SetSprite();
	content.push_back(obj1);*/
	/*HARDCORE END*/
	sprite = nullptr;
};

void TileGrid::Draw(sf::RenderWindow *window)
{
	Texture *t = textures.begin()->get();
	t->SetInfo(0, 0, true, 1);
	Sprite spr(textures.begin()->get(), 0, 1, 0);
	//spr.Draw(window, 100, 100);

	Tile *firstTile = this->GetTile(1, 1);
	firstTile->SetSprite(1, 0, 0);
	firstTile->Draw(window);
	/*Tile * firstTile = this->GetTile(1, 1);
	firstTile -> Draw(window);*/

}

void Tile::Draw(sf::RenderWindow *window)
{
	auto f = this->GetSprite();
	f->Draw(window, 100, 100);
	int i = 0;
	for (auto &object : content) {
		object->GetSprite()->Draw(window, 100, 100);
		cout << i << endl;

	}
}


Tile* Block::GetTile(int x, int y) {
	if (x >= 0 && x < tileGrid->BlockSize && y >= 0 && y < tileGrid->BlockSize) 
		return tiles[y][x];
	cout << "Can't return tile " << x << ", " << y << endl;
	return nullptr;
}