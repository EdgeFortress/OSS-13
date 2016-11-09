#pragma once

class Block;

struct Diff {
    Block *block;
    int x, y;
    int objectNum;
    explicit Diff(Block *block, int x, int y, int objectNum) : 
        block(block),
        x(x), y(y),
        objectNum(objectNum) 
    { }
};

struct MoveDiff : public Diff {
    int toX, toY;
    explicit MoveDiff(Block *block, int x, int y, int objectNum, int toX, int toY) :
        Diff(block, x, y, objectNum),
        toX(toX), toY(toY)
    { }
};