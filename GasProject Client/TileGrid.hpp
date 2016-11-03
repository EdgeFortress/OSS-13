#pragma once

#include <vector>

#include "Client.hpp"
#include "Sprite.hpp"
#include "State.hpp"

class Block;
class TileGrid;

using namespace std;

class Object {
private:
    Sprite *sprite;

public:
    Object() { sprite = new Sprite(); }

    void SetSprite(int textureIndex, int num, int direction, int frame);
    Sprite *GetSprite() { return sprite; }
    ~Object()
    {
        if (sprite) delete sprite;
    }
};

class Tile {
private:
	Block *block;
	Sprite *sprite;
	list<uptr<Object>> content;

public:
	explicit Tile(Block *block);

	Tile(const Tile &) = delete;
	Tile &operator=(const Tile &) = delete;
	~Tile() = default;

	void Clear() { 
		content.clear();
	}
	void AddObject(Object *obj) {
		content.push_back(uptr<Object>(obj));
	}
	Sprite *GetSprite() const { return sprite;  }
	void SetSprite(int, int, int);
	void Draw(sf::RenderWindow *);

	//const list<uptr<Object>> &GetContent() const { return content; }
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
	list<uptr<Texture>> &textures;
	vector< vector<Block *> > blocks;

public:
	static const int BlockSize = 8;
	static const int NumOfBlocks = 4;//non static is non visible for constructor
	static const int SpriteRes = 64;

	explicit TileGrid(list<uptr<Texture>> &textures);
	void Initialize();

	TileGrid(const TileGrid &) = delete;
	TileGrid &operator=(const TileGrid &) = delete;
	~TileGrid() = default;

	Tile *GetTile(int x, int y)
	{

		if (x >= 0 && x < NumOfBlocks * BlockSize && y >= 0 && y < NumOfBlocks * BlockSize)
			return blocks[y / BlockSize][x / BlockSize]->GetTile(x % BlockSize, y % BlockSize);
		CC::log << "Can't return block " << x << ", " << y << endl;
		return nullptr;
	}

	void Draw(sf::RenderWindow *);
};