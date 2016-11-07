#pragma once

#include <vector>

#include "Client.hpp"
#include "Graphics/Sprite.hpp"
#include "State.hpp"

namespace sf {
    class Packet;
}

class Block;
class TileGrid;

using namespace std;

class Object {
private:
    Global::Sprite sprite;
    int direction;

public:
    Object() : sprite(Global::Sprite::EMPTY), direction(-1) { }

    void SetSprite(Global::Sprite sprite) { this->sprite = sprite; };
    Global::Sprite GetSprite() const { return sprite; }

    friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
};

class Tile {
private:
    Block *block;
    Global::Sprite sprite;
    list<uptr<Object>> content;

public:
    explicit Tile(Block *block);

    Tile(const Tile &) = delete;
    Tile &operator=(const Tile &) = delete;
    ~Tile() = default;

    void Draw(sf::RenderWindow *, int x, int y);

    void Clear() { content.clear(); }
    void AddObject(Object *obj) { content.push_back(uptr<Object>(obj)); }

    Global::Sprite GetSprite() const { return sprite;  }
    void SetSprite(Global::Sprite sprite) { this->sprite = sprite; };

    //const list<uptr<Object>> &GetContent() const { return content; }

    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
};

class Block {
private:
    TileGrid *tileGrid;
    vector< vector< uptr<Tile> > > tiles;

public:
    explicit Block(TileGrid *tileGrid);

    Block(const Block &) = delete;
    Block &operator=(const Block &) = delete;
    ~Block() = default;

    Tile *GetTile(int x, int y) const;

    friend sf::Packet &operator>>(sf::Packet &packet, Block &block);
};

class TileGrid {
private:
    int xNumOfTiles, yNumOfTiles;
    int xPos, yPos;
    int blockSize;

    std::mutex mutex;
    vector< vector<uptr<Block>> > blocks;

public:
    explicit TileGrid();

    TileGrid(const TileGrid &) = delete;
    TileGrid &operator=(const TileGrid &) = delete;
    ~TileGrid() = default;

    Tile *GetTile(int x, int y) const;
    void Draw(sf::RenderWindow *);

    void Lock() { mutex.lock(); }
    void Unlock() { mutex.unlock(); }

    int GetBlockSize() const { return blockSize; }

    friend sf::Packet &operator>> (sf::Packet &packet, TileGrid &tileGrid);
};