#include "Turf.hpp"

#include "Server.hpp"

void Turf::Interact(Object *) {
    Server::log << "Interact" << name << std::endl;
};