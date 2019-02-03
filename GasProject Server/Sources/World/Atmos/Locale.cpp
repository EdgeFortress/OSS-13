#include "Locale.hpp"

#include <Server.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>

#include "Atmos.hpp"

Locale::Locale(Atmos *atmos, Tile *tile) :
    atmos(atmos), gases(int(Gas::Count), 0),
    closed(true)
{
    tiles.push_back(tile);
    tile->locale = this;
}

void Locale::Update(sf::Time timeElapsed) {
    if (needToCheckCloseness) {
        for (auto tile : tiles) {
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    Tile *neighbour = tile->GetMap()->GetTile({ tile->GetPos().x + dx, tile->GetPos().y + dy });
                    if (!neighbour ||
                        dx == 0 && dy == 0)
                        continue;
                    if (neighbour->IsSpace()) {
                        closed = false;
                        return;
                    }
                }
        }
        closed = true;
    }
}

void Locale::AddTile(Tile* tile) {
    if (!tile) {
        Server::log << "Error: try to add nullptr to Locale" << std::endl;
        return;
    }
    if (tile->locale == this) {
        Server::log << "Warning: try to add tile to Locale twice" << std::endl;
        return;
    }
    tiles.push_back(tile);
    tile->locale = this;
}

void Locale::RemoveTile(Tile* tile) {
    if (!tile) {
        Server::log << "Error: try to remove nullptr from Locale" << std::endl;
        return;
    }

    for (auto iter = tiles.begin(); iter != tiles.end(); iter++) {
        if (*iter == tile) {
            tiles.erase(iter);
            tile->locale = nullptr;
            return;
        }
    }
    Server::log << "Warning: try to remove tile from Locale, but it doesn't contain it" << std::endl;
}

void Locale::Merge(Locale *locale) {
    if (locale == this) return;
    for (auto tile : locale->tiles) {
        AddTile(tile);
    }
    locale->tiles.clear();
    atmos->RemoveLocale(locale);
}

void Locale::Clear() {
    for (auto tile: tiles) {
        tile->locale = nullptr;
    }
    tiles.clear();
    closed = true;
}

void Locale::Open() {
    closed = false;
}

void Locale::CheckCloseness() {
    needToCheckCloseness = true;
}

bool Locale::IsEmpty() const { return tiles.empty(); }
bool Locale::IsClosed() const { return closed; }
uint Locale::NumOfTiles() const { return uint(tiles.size()); }

