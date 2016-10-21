#pragma once

#include <vector>

#include "Client.hpp"
#include "Sprite.hpp"
#include "State.hpp"

class Block;
class Object;
class TileGrid;

using namespace std;

class Tile {
private:
	Block *block;
	Sprite *sprite;
	list<Object *> content;
	ClientController *clientController;

public:
	explicit Tile(Block *block) : block(block) {
		sprite = nullptr;
	};

	Tile(const Tile &) = delete;
	Tile &operator=(const Tile &) = delete;
	~Tile() = default;

	Sprite *GetSprite() { return sprite;  }
	void SetSprite(int, int, int);

	list<Object *> GetContent() { return content; }

	void Clean()
	{
		if (sprite) {
			delete sprite;
			sprite = nullptr;
		}
	}
};

class Block {
private:
	TileGrid *tileGrid;
	vector < vector<Tile *> > tiles;

public:
	explicit Block(TileGrid *tileGrid);

	Block(const Block &) = delete;
	Block &operator=(const Block &) = delete;
	~Block() = default;

	Tile *GetTile(int x, int y);
};

class TileGrid {
private:
	int xNumOfTiles, yNumOfTiles;
	int xZero, yZero;
	int xPos, yPos;

	vector< vector<Block *> > blocks;

public:
	const int BlockSize = 8;
	const int NumOfBlocks = 4;
	const int SpriteRes = 64;

	explicit TileGrid() : blocks(NumOfBlocks, vector<Block *>(NumOfBlocks)) {
		for (auto &vect : blocks)
			for (auto &block : vect)
				block = new Block(this);
	}

	TileGrid(const TileGrid &) = delete;
	TileGrid &operator=(const TileGrid &) = delete;
	~TileGrid() = default;

	Tile *GetTile(int x, int y)
	{
		if (x >= 0 && x < NumOfBlocks * BlockSize && y >= 0 && y < NumOfBlocks * BlockSize) return blocks[y / BlockSize][x / BlockSize]->GetTile(x % BlockSize, y % BlockSize);
		cout << "Can't return block " << x << ", " << y << endl;
		return nullptr;
	}

	void Draw(sf::RenderWindow *);
};

class Object {
private:
	Sprite *sprite;
	ClientController *clientController;

public:
	Object() { sprite = new Sprite(); }

	void SetSprite(int, int, int, int);
	Sprite *GetSprite() { return sprite; }
	~Object()
	{
		if (sprite) delete sprite;
	}
};