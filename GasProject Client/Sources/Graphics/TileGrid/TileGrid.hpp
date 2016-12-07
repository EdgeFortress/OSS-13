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
    string name;
    explicit Object(const Global::Sprite key = Global::Sprite::EMPTY, const string name = "", const bool directed = false);

    void Draw(sf::RenderWindow * const, const int x, const int y);

    void SetSprite(const Global::Sprite);

    bool checkObj(int x, int y);
    Sprite *GetSprite() const { return sprite; }

    friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
};

class Tile {
private:
    Block *block;
    const int x, y;
    Sprite *sprite;
    list<uptr<Object>> content;

public:
    explicit Tile(Block *block, const int x, const int y);

    Tile(const Tile &) = delete;
    Tile &operator=(const Tile &) = delete;
    ~Tile() = default;

    void Draw(sf::RenderWindow * const, const int x, const int y) const;

    Object *GetObjectByCoord(const unsigned x, const unsigned y) const;
    void Clear() { content.clear(); sprite = nullptr; }
    void AddObject(Object *obj, int num) {
        if (num > content.size()) {
            CC::log << "Wrong object number" << endl;
            return;
        }
        auto iter = content.begin();
        for (int i = 0; i < num; iter++, i++);
        content.insert(iter, uptr<Object>(obj));
    }
    uptr<Object> RemoveObject(int num) {
        if (num >= content.size() || num < 0) {
            CC::log << "Wrong object num for remove: tile" << "(" << x << "," << y << ") num: " << num << endl;
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

    const list<uptr<Object>> &GetContent() const { return content; }

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
    // Camera position
    int xPos, yPos;
    int xRelPos, yRelPos;
    int tileSize;
    // TileGrid padding
    int xPadding, yPadding;

    int blockSize;
    int firstBlockX, firstBlockY;
    int firstTileX, firstTileY;
    int numOfVisibleBlocks;

    std::mutex mutex;
    // Protection against re-locking
    bool drawingLocked;

    vector< vector<sptr<Block>> > blocks;

public:
    explicit TileGrid();

    TileGrid(const TileGrid &) = delete;
    TileGrid &operator=(const TileGrid &) = delete;
    ~TileGrid() = default;

    void Draw(sf::RenderWindow * const);
    void Resize(const int windowWidth, const int windowHeight);

    //void Lock() { mutex.lock(); }
    //void Unlock() { mutex.unlock(); }

    // Lock for network updating
    void LockDrawing();
    // Unlock for network updating
    void UnlockDrawing();

    // Differences commiting
    void Move(int blockX, int blockY, int x, int y, int objectNum, int toX, int toY, int toObjectNum);
    void Add(int blockX, int blockY, int x, int y, int objectNum, Global::Sprite sprite, string name);
    void Remove(int blockX, int blockY, int x, int y, int objectNum);

    void ShiftBlocks(const int newFirstX, const int newFirstY);

    void SetCameraPosition(const int x, const int y);
    void SetBlock(int x, int y, Block *);

    wptr<Block> GetBlock(const int blockX, const int blockY) const;
    Tile *GetTile(int x, int y) const;
    Object *GetObjectByPixel(int x, int y) const;

    const int GetBlockSize() const { return blockSize; }
    const int GetTileSize() const { return tileSize; }
    const int GetWidth() const { return tileSize * xNumOfTiles; }
    const int GetHeigth() const { return tileSize * yNumOfTiles; }
    const int GetPaddingX() const { return xPadding; }
    const int GetPaddingY() const { return yPadding; }

    friend sf::Packet &operator>> (sf::Packet &packet, TileGrid &tileGrid);
};