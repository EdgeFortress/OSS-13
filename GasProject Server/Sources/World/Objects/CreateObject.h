#pragma once

#include <string>

#include <Shared/Types.hpp>

class Tile;
class Object;

Object *CreateObject(const std::string &module, Tile *tile = nullptr);
//Object *CreateObject(const std::string &module, uf::vec2i pos);
//Object *CreateObject(const std::string &module, apos coords);
