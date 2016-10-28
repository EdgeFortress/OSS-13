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
	sf::Sprite sprite;
	sprite.setTexture(*(textures.begin()->get()->GetSFMLTexture()));
	window->draw(sprite);
	/*Tile * firstTile = this->GetTile(1, 1);
	firstTile -> Draw(window);*/

}

void Tile::Draw(sf::RenderWindow *window)
{
	int i = 0;
	for (auto &object : content) {
		i++;
		cout << i << endl;

	}
}


Tile* Block::GetTile(int x, int y) {
	if (x >= 0 && x < tileGrid->BlockSize && y >= 0 && y < tileGrid->BlockSize) 
		return tiles[y][x];
	cout << "Can't return tile " << x << ", " << y << endl;
	return nullptr;
}