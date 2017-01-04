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

struct MoveDiff : public Diff {
    Block *lastBlock;
    Object *object;
    int toX, toY, toObjectNum;
    MoveDiff(uint id, int toX, int toY, int toObjectNum, Block *lastBlock, Object* object) :
        Diff(id), lastBlock(lastBlock),
        toX(toX), toY(toY), toObjectNum(toObjectNum)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::MOVE; }
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

struct ShiftDiff : public Diff {
    Global::Direction direction;
    float speed;
    ShiftDiff(uint id, Global::Direction direction, float speed) :
        Diff(id), direction(direction), speed(speed)
    { }

    virtual Global::DiffType GetType() const final { return Global::DiffType::SHIFT; }
};

