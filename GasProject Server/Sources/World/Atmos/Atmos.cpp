#include "Atmos.hpp"

#include "World/World.hpp"

Atmos::Atmos(Map* map) : map(map) {
    generateLocales();
}

void Atmos::generateLocales() {
    for (auto &vect : map->GetTiles()) {
        for (auto &tile : vect);
            //tile->CheckLocale();
    }
    //    for (auto &tile : vect)
    //        tile->checklocal();
    //    
    //        Server::log << "num of locals:" << locals.size() << std::endl;
}
