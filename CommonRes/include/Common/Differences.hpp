#pragma once

#include "Server.hpp"

class Block;

struct Diff {
    enum class Type : char {
        NONE = 0,
        MOVE,
        ADD,
        REMOVE
    };

    Block *block;
    int x, y;
    int objectNum;

    explicit Diff(Block *block, int x, int y, int objectNum) : 
        block(block),
        x(x), y(y),
        objectNum(objectNum)
    {
        
    }

    virtual Type GetType() const { return Type::NONE; }
};

struct MoveDiff : public Diff {
    int toX, toY;
    explicit MoveDiff(Block *block, int x, int y, int objectNum, int toX, int toY) :
        Diff(block, x, y, objectNum),
        toX(toX), toY(toY)
    { }

    virtual Type GetType() const final { return Type::MOVE; }
};

struct AddDiff : public Diff {
    explicit AddDiff(Block *block, int x, int y, int objectNum) :
        Diff(block, x, y, objectNum)
    { }

    virtual Type GetType() const final { return Type::ADD; }
};

struct RemoveDiff : public Diff {
    explicit RemoveDiff(Block *block, int x, int y, int objectNum) :
        Diff(block, x, y, objectNum)
    { }
        
    virtual Type GetType() const final { return Type::REMOVE;; }
};