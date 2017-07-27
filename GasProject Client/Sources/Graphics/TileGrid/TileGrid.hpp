#pragma once

#include <vector>
#include <list>
#include <mutex>

#include "Client.hpp"
#include "Graphics/Sprite.hpp"
#include "State.hpp"

#include "Shared/Types.hpp"

namespace sf {
    class Packet;
    class Event;
    class Time;
}

class Tile;
class Block;
class TileGrid;

using namespace std;

class Object {
private:
    uint id;
    string name;
    Sprite *sprite;
    uf::Direction direction;
    int layer;
    bool dense;

    sf::Vector2f shift;
    sf::Vector2i shiftingDirection;
    float shiftingSpeed;

    Tile *tile;

public:
    explicit Object(const Global::Sprite key = Global::Sprite::EMPTY, const string name = "", const bool directed = false);

    void Draw(sf::RenderWindow * const, const int x, const int y);
    void Update(sf::Time timeElapsed);

    void SetSprite(const Global::Sprite);
	void SetDirection(const uf::Direction);
	void SetSpeed(float speed);
    void SetShifting(uf::Direction direction, float speed) {
        auto directionVect = sf::Vector2i(uf::DirectionToVect(direction));
		if (shiftingDirection != directionVect) shiftingDirection += directionVect;
        shiftingSpeed = speed;
    }
    void ResetShifting() {
        shiftingDirection = sf::Vector2i(0, 0);
        shift = sf::Vector2f(0, 0);
        shiftingSpeed = 0;
    }
    void ReverseShifting(uf::Direction direction);

    bool checkObj(int x, int y);

    uint GetID() const { return id; }
    string GetName() const { return name; }
    Sprite *GetSprite() const { return sprite; }
    int GetLayer() const { return layer; }

    Tile *GetTile() { return tile; }
    sf::Vector2f GetShift() const { return shift; }
    sf::Vector2i GetShiftingDirection() const { return shiftingDirection; }
    bool IsDense() { return dense; }

    friend sf::Packet &operator>>(sf::Packet &packet, Object &object);
    friend Tile;
};

class Tile {
private:
    Block *block;
    const int x, y;
    Sprite *sprite;
    list<Object *> content;

public:
    Tile(Block *block, const int x, const int y);
    Tile(const Tile &) = delete;
    Tile &operator=(const Tile &) = delete;
	~Tile();

    void Draw(sf::RenderWindow * const, const int x, const int y) const;
    void Update(sf::Time timeElapsed) {
        //for (auto &obj : content)
        //    obj->Update(timeElapsed);
    }

    Object *GetObjectByCoord(const unsigned x, const unsigned y) const;
    void Clear() { 
        for (auto &obj : content)
            obj->tile = nullptr;
        content.clear();
        sprite = nullptr; 
    }

    void AddObject(Object *obj, int num = -1) {
        if (num < content.size() && num >= 0) {
            auto iter = content.begin();
            for (int i = 0; i < num && iter != content.end(); iter++, i++);
            content.insert(iter, obj);
        } else {
            content.push_back(obj);
        }
        if (obj->tile) {
            //int dx = GetRelX() - obj->tile->GetRelX();
            //int dy = GetRelY() - obj->tile->GetRelY();
            //if (obj->shift.x && dx) obj->shift.x -= sgn(dx);
            //if (obj->shift.y && dy) obj->shift.y -= sgn(dy);
            obj->tile->RemoveObject(obj);
        }
        obj->tile = this;
    }

    Object *RemoveObject(uint id) {
        for (auto iter = content.begin(); iter != content.end(); iter++)
            if ((*iter)->id == id) {
                Object *obj = (*iter);
                content.erase(iter);
                obj->tile = nullptr;
                return obj;
            }
        return nullptr;
    }

    Object *RemoveObject(Object *obj) {
        content.remove(obj);
        obj->tile = nullptr;
        return obj;
    }

    Object *GetObject(uint id) {
        for (auto iter = content.begin(); iter != content.end(); iter++)
            if ((*iter)->id == id)
                return *iter;
        return nullptr;
    }

    bool IsBlocked() {
        for (auto &obj : content)
            if (obj->IsDense()) return true;
        if (!sprite) return true;
        return false;
    }

    int GetRelX() const;
    int GetRelY() const;
    const list<Object *> &GetContent() const { return content; }

    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
    friend TileGrid;
};

class Block {
private:
    TileGrid *tileGrid;
    int relX, relY;
    //int id;

    vector< vector< uptr<Tile> > > tiles;

public:
    explicit Block(TileGrid *tileGrid);

    Block(const Block &) = delete;
    Block &operator=(const Block &) = delete;
    ~Block() = default;
	//~Block() { CC::log << "Block removed" << relX << relY << std::endl; }

    void Update(sf::Time timeElapsed) {
        for (auto &vect : tiles)
            for (auto &tile : vect)
                tile->Update(timeElapsed);
    }

    int GetRelX() const { return relX; }
    int GetRelY() const { return relY; }
    Tile *GetTile(int x, int y) const;
    TileGrid *GetTileGrid() const { return tileGrid; }

    friend sf::Packet &operator>>(sf::Packet &packet, Block &block);
    friend TileGrid;
};

class TileGrid {
private:
    int xNumOfTiles, yNumOfTiles;
    int tileSize;

    // Camera position
    int xPos, yPos;
    int xRelPos, yRelPos;
    sf::Vector2f shift;
    
    // TileGrid padding
    int xPadding, yPadding;

    int blockSize;
    int firstBlockX, firstBlockY;
    int firstTileX, firstTileY;
    int numOfVisibleBlocks;

    std::mutex mutex;

    vector< vector< sptr<Block> > > blocks;
    list< uptr<Object> > objects;

    // Controls
    Object *controllable;
    float controllableSpeed;
    const sf::Time MOVE_TIMEOUT = sf::milliseconds(100);
    sf::Vector2i moveCommand;
    sf::Time moveSendPause;
	bool ghostButtonPressed;

public:
    explicit TileGrid();

    TileGrid(const TileGrid &) = delete;
    TileGrid &operator=(const TileGrid &) = delete;
    ~TileGrid() = default;

    void Draw(sf::RenderWindow * const);
    void Resize(const int windowWidth, const int windowHeight);
    void HandleEvent(sf::Event event);
    void Update(sf::Time timeElapsed);

    //// FOR NETWORK

        // Lock for network updating
        void LockDrawing();
        // Unlock for network updating
        void UnlockDrawing();

        // Differences commiting
        //void Move(int blockX, int blockY, int x, int y, int objectNum, int toX, int toY, int toObjectNum);
        //void Add(int blockX, int blockY, int x, int y, int objectNum, Global::Sprite sprite, string name);
        //void Remove(int blockX, int blockY, int x, int y, int objectNum);
        //void Shift(int blockX, int blockY, int x, int y, int objectNum, uf::Direction direction, float speed);

        void AddObject(Object *object);
        void RemoveObject(uint id);
        void RelocateObject(uint id, int toX, int toY, int toObjectNum);
        void MoveObject(uint id, uf::Direction direction, float speed);
		void ChangeObjectDirection(uint id, uf::Direction direction);

        void ShiftBlocks(const int newFirstX, const int newFirstY);

        void SetCameraPosition(const int x, const int y);
        void SetBlock(int x, int y, Block *);
        void SetControllable(uint id, float speed);

    ////

    wptr<Block> GetBlock(const int blockX, const int blockY) const;
    Tile *GetTileRel(int x, int y) const;
    Tile *GetTileAbs(int x, int y) const;
    Object *GetObjectByPixel(int x, int y) const;

    const int GetBlockSize() const { return blockSize; }
    const int GetTileSize() const { return tileSize; }
    const int GetWidth() const { return tileSize * xNumOfTiles; }
    const int GetHeigth() const { return tileSize * yNumOfTiles; }
    const int GetPaddingX() const { return xPadding; }
    const int GetPaddingY() const { return yPadding; }

    friend sf::Packet &operator>>(sf::Packet &packet, TileGrid &tileGrid);
    friend sf::Packet &operator>>(sf::Packet &packet, Tile &tile);
};