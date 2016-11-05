#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "TileGrid.hpp"

void Object::SetSprite(int textureIndex, int num, int direction, int frame)
{
    for (const uptr<Texture> &texture : ClientController::Get()->GetWindow()->GetTextures())
        if (texture->GetKey() == textureIndex) {
            sprite->SetTexture(texture.get());
            sprite->SetSpriteState(num, direction, frame);
            return;
        }
}

Tile::Tile(Block *block) : block(block) {
	sprite = nullptr;
};

void Tile::SetSprite(int textureIndex, int num, int frame) {
    for (const uptr<Texture> &texture : ClientController::Get()->GetWindow()->GetTextures())
        if (texture->GetKey() == textureIndex) {
            sprite = new Sprite();
            sprite->SetTexture(texture.get());
            sprite->SetSpriteState(num, 0, frame);
            return;
        }
}

void Tile::Draw(sf::RenderWindow *window,int sizeTile,int xNum = 0, int yNum = 0)
{
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
    }
}

Tile* Block::GetTile(int x, int y) {
    if (x >= 0 && x < tileGrid->BlockSize && y >= 0 && y < tileGrid->BlockSize)
        return tiles[y][x];
    CC::log << "Can't return tile " << x << ", " << y << endl;
    return nullptr;
}

void TileGrid::Draw(sf::RenderWindow *window)
{
	for(int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			this->GetTile(xPos + i, yPos + j)->Draw(window, sizeTile, i, j);
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
	xPos = 1;
	yPos = 0;

}