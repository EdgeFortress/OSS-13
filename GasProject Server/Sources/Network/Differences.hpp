#pragma once

#include <string>

#include "Common/NetworkConst.hpp"
#include "Common/Useful.hpp"
#include "TileGrid_Info.hpp"

class Block;
class Object;

struct Diff {
    uint id;

    Diff(uint id = -1) : id(id)
    { }

    virtual Global::DiffType GetType() const { return Global::DiffType::NONE; }
};

struct ReplaceDiff : public Diff {
    Block *lastBlock;
    Object *object;
    int toX, toY, toObjectNum;
    ReplaceDiff(uint id, int toX, int toY, Block *lastBlock, Object *object) :
        Diff(id), lastBlock(lastBlock),
        toX(toX), toY(toY),
        object(object)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::RELOCATE; }
};

struct AddDiff : public Diff {
    ObjectInfo objectInfo;
    AddDiff(ObjectInfo objectInfo) : objectInfo(objectInfo)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::ADD; }
};

struct RemoveDiff : public Diff {
    RemoveDiff(uint id) : Diff(id)
    { }
        
    virtual Global::DiffType GetType() const final { return Global::DiffType::REMOVE; }
};

struct MoveDiff : public Diff {
    Global::Direction direction;
    Block *lastblock;
    MoveDiff(uint id, Global::Direction direction, Block *lastblock) :
        Diff(id), direction(direction), lastblock(lastblock)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::MOVE; }
};

struct ShiftDiff : public Diff {
    Global::Direction direction;
    float speed;
    
    ShiftDiff(uint id, Global::Direction direction, float speed) :
        Diff(id), direction(direction), speed(speed)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::SHIFT; }
};

