#include "Camera.hpp"
#include "Common/Useful.hpp"
#include "Server.hpp"
#include "World.hpp"

Camera::Camera() {
    suspense = true;
    tile = nullptr;
}

Camera::Camera(const Tile * const tile) : suspense(true) {
    this->tile = tile;
    countVisibleBlocks();
    Server::log << "Camera created: " << tile->X() << " " << tile->Y() << endl;
    suspense = false;
}

void Camera::SetPosition(const Tile * const tile) {
    this->tile = tile;
    suspense = false;
    countVisibleBlocks();
}

void Camera::Suspend() {
    tile = nullptr;
    suspense = true;
}

void Camera::countVisibleBlocks() {
    //Server::log << "Count visible blocks" << endl;

    visibleBlocks.resize(0);
    if (!tile) return;

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
    Server::log << "First visible block: " << firstBlockX << ", " << firstBlockY << endl;

    relX = tile->X() - firstBlockX * Global::BLOCK_SIZE;
    relY = tile->Y() - firstBlockY * Global::BLOCK_SIZE;
    Server::log << "Related camera pos: " << relX << ", " << relY << endl;

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

const std::list<sptr<Diff>> Camera::GetVisibleDifferences() {
    std::list<sptr<Diff>> diffs(0);
    if (suspense) return diffs;
    for (auto &vect : visibleBlocks)
        for (auto &block : vect) {
            if (block) {
                for (auto &diff : block->GetDifferences())
                    diffs.push_back(diff);
            }
        }

    return diffs;
}