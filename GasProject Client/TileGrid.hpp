#pragma once

#include <vector>

using namespace std;

class Block;
class TileGrid;

class Tile {
private:
	Block *block;
public:
	explicit Tile(Block *block) : block(block) {

	};

	Tile(const Tile &) = delete;
	Tile &operator=(const Tile &) = delete;
	~Tile() = default;
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
};

class TileGrid {
private:
	vector< vector<Block *> > blocks;

public:
	const int BlockSize = 8;
	const int NumOfBlocks = 4;

	explicit TileGrid() : blocks(NumOfBlocks, vector<Block *>(NumOfBlocks)) {
		for (auto &vect : blocks)
			for (auto &block : vect)
				block = new Block(this);
	}

	TileGrid(const TileGrid &) = delete;
	TileGrid &operator=(const TileGrid &) = delete;
	~TileGrid() = default;

	void Draw() const {

	}
};