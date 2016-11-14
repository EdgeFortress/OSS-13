#pragma once

#include <vector>
#include <list>

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
    Sprite *sprite;
    int direction;

public:
    explicit Object(const Global::Sprite key = Global::Sprite::EMPTY, const bool directed = false);

    void Draw(sf::RenderWindow * const, const int x, const int y);

    void SetSprite(const Global::Sprite);
    //Sprite *GetSprite() const { return sprite; }

    friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
};

class Tile {
private:
    Block *block;
    const int x, y;
    //Global::Sprite sprite;
    Sprite *sprite;
    list<uptr<Object>> content;

public:
    explicit Tile(Block *block, const int x, const int y);

    Tile(const Tile &) = delete;
    Tile &operator=(const Tile &) = delete;
    ~Tile() = default;

    void Draw(sf::RenderWindow * const, const int x, const int y) const;

    void Clear() { content.clear(); sprite = nullptr; }
    void AddObject(Object *obj) { content.push_back(uptr<Object>(obj)); }
    uptr<Object> RemoveObject(int num) { 
        if (num >= content.size() || num < 0) {
            CC::log << "Wrong object num: tile" << "(" << x << "," << y << ") num: " << num << endl;
            return uptr<Object>();
        }
        for (auto &obj : content) {
            if (!num) {
                uptr<Object> temp = std::move(obj);
                content.remove(obj);
                return std::move(temp);
            }
            else num--;
        }
        return uptr<Object>();
    }

    //Global::Sprite GetSprite() const { return sprite;  }
    //void SetSprite(Global::Sprite sprite) { this->sprite = sprite; };

    //const list<uptr<Object>> &GetContent() const { return content; }

    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
};

class Block {
private:
    TileGrid *tileGrid;
    int id;

    vector< vector< uptr<Tile> > > tiles;

public:
    explicit Block(TileGrid *tileGrid);

    Block(const Block &) = delete;
    Block &operator=(const Block &) = delete;
    ~Block() = default;

    int GetID() const { return id; }
    Tile *GetTile(int x, int y) const;

    friend sf::Packet &operator>>(sf::Packet &packet, Block &block);
};

class TileGrid {
private:
    int xNumOfTiles, yNumOfTiles;
    int xPos, yPos;
    int blockSize;
    int tileSize;
    int xPadding, yPadding;

    std::mutex mutex;
    vector< vector<uptr<Block>> > blocks;

public:
    explicit TileGrid(const int windowWidth, const int windowHeight);

    TileGrid(const TileGrid &) = delete;
    TileGrid &operator=(const TileGrid &) = delete;
    ~TileGrid() = default;

    void Draw(sf::RenderWindow * const);

    void Lock() { mutex.lock(); }
    void Unlock() { mutex.unlock(); }

    void Resize(const int windowWidth, const int windowHeight);

    // Differences commiting
    void Move(int blockID, int x, int y, int objectNum, int toX, int toY);
    void Add() {}
    void Remove() {}

    Block *GetBlock(int blockID) const;
    Tile *GetTile(int x, int y) const;

    const int GetBlockSize() const { return blockSize; }
    const int GetTileSize() const { return tileSize; }

    friend sf::Packet &operator>> (sf::Packet &packet, TileGrid &tileGrid);
};