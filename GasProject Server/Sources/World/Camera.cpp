#include "Camera.hpp"
#include "Common/Useful.hpp"
#include "Server.hpp"
#include "World.hpp"

Camera::Camera() {
    suspense = true;
    tile = nullptr;
}

Camera::Camera(Tile *tile) {
    this->tile = tile;
    suspense = false;
    countVisibleBlocks();
    Server::log << "Camera created: " << tile->X() << " " << tile->Y() << endl
        << "Visible blocks: " << endl;
    for (auto &vect : visibleBlocks) {
        for (auto *&block : vect) {
            if (block)
                Server::log << block->X() << " " << block->Y() << endl;
        }
    }
}

void Camera::SetPosition(Tile *tile) {
    this->tile = tile;
    suspense = false;
    countVisibleBlocks();
}

void Camera::Suspend() {
    tile = nullptr;
    suspense = true;
}

void Camera::countVisibleBlocks() {
    visibleBlocks.resize(0);
    if (suspense) return;

    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    int num_of_visible_blocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    num_of_visible_blocks = num_of_visible_blocks / Global::BLOCK_SIZE + (num_of_visible_blocks % Global::BLOCK_SIZE ? 1 : 0);

    Server::log << "Num of visible blocks: " << num_of_visible_blocks << endl;

    // Count coords of first visible tile, and block
    int firstTileX = tile->X() - Global::FOV / 2 - Global::MIN_PADDING;
    int firstTileY = tile->Y() - Global::FOV / 2 - Global::MIN_PADDING;
    int firstBlockX = firstTileX / Global::BLOCK_SIZE + (firstTileX < 0 ? -1 : 0);
    int firstBlockY = firstTileY / Global::BLOCK_SIZE + (firstTileY < 0 ? -1 : 0);
    Server::log << "First visible tile: " << firstTileX << ", " << firstTileY << endl;

    // Filling our result vector by block pointers
    visibleBlocks.resize(num_of_visible_blocks);
    int y = firstBlockY;
    for (auto &vect : visibleBlocks) {
        vect.resize(num_of_visible_blocks);
        int x = firstBlockX;
        for (auto *&block : vect) {
            block = tile->GetMap()->GetBlock(x, y);
            x++;
        }
        y++;
    }
}