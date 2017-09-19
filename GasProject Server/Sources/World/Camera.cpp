#include "Shared/Command.hpp"

#include "Camera.hpp"

#include "Server.hpp"
#include "World.hpp"
#include "Objects/Control.hpp"
#include "Player.hpp"
#include "Network/Differences.hpp"

Camera::Camera(const Tile * const tile) :
    tile(nullptr), lasttile(nullptr), suspense(true),
    changeFocus(false),
    unsuspensed(false), cameraMoved(false)
{
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    visibleBlocksNum = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    visibleBlocksNum = visibleBlocksNum / Global::BLOCK_SIZE + (visibleBlocksNum % Global::BLOCK_SIZE ? 1 : 0);

    visibleBlocks.resize(visibleBlocksNum);
    for (auto &vect : visibleBlocks)
        vect.resize(visibleBlocksNum);

    blocksSync.resize(visibleBlocksNum);
    for (auto &vect : blocksSync)
        vect = std::vector<bool>(visibleBlocksNum, false);

    SetPosition(tile);
}

void Camera::UpdateView() {
    if (unsuspensed && cameraMoved) 
        Server::log << "Logic error: camera unsuspensed and moved at one time" << std::endl;

    int updateOptions = GraphicsUpdateServerCommand::Option::EMPTY;
    GraphicsUpdateServerCommand *command = new GraphicsUpdateServerCommand();

    if (unsuspensed || cameraMoved) {
        if (unsuspensed) fullRecountVisibleBlocks(tile);
        else refreshVisibleBlocks(tile);
        updateOptions |= GraphicsUpdateServerCommand::Option::CAMERA_MOVE;
        command->cameraX = tile->X();
        command->cameraY = tile->Y();
    }

    unsuspensed = cameraMoved = false;

    for (uint x = 0; x < visibleBlocksNum; x++)
        for (uint y = 0; y < visibleBlocksNum; y++) {
            Block *block = visibleBlocks[y][x];
            if (block) {
                if (blocksSync[y][x]) {
                    for (auto &diff : block->GetDifferences()) {
						// Check diff visibility
						if (!diff->CheckVisibility(seeInvisibleAbility)) continue;

                        // If client doesn't know about moved object, we need to add it
                        if (diff->GetType() == Global::DiffType::RELOCATE) {
                            ReplaceDiff *replaceDiff = dynamic_cast<ReplaceDiff *>(diff.get());
                            Block *lastBlock = replaceDiff->lastBlock;
                            if (!lastBlock ||
                                lastBlock->X() < firstBlockX || lastBlock->X() >= firstBlockX + int(visibleBlocksNum) ||
                                lastBlock->Y() < firstBlockY || lastBlock->Y() >= firstBlockY + int(visibleBlocksNum)) 
                            {
                                command->diffs.push_back(std::make_shared<Diff>(*replaceDiff));
                            }
                        }
                        //if (diff->GetType() == Global::DiffType::SHIFT) {
                        //    continue;
                        //}
                        command->diffs.push_back(diff);
                    }
                } else {
                    command->blocksInfo.push_back(block->GetBlockInfo(seeInvisibleAbility));
                    blocksSync[y][x] = true;
                }
            }
        }

    if (blockShifted) {
        updateOptions |= GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT;
        command->firstBlockX = firstBlockX;
        command->firstBlockY = firstBlockY;
        blockShifted = false;
    }

    if (command->diffs.size()) {
        updateOptions |= GraphicsUpdateServerCommand::Option::DIFFERENCES;
    }

    if (changeFocus) {
        updateOptions |= GraphicsUpdateServerCommand::Option::NEW_CONTROLLABLE;
        command->controllable_id = player->GetControl()->GetOwner()->ID();
        command->controllableSpeed = player->GetControl()->GetSpeed();
        changeFocus = false;
    }

    command->options = GraphicsUpdateServerCommand::Option(updateOptions);
    
    if (updateOptions)
        player->AddCommandToClient(command);
}

void Camera::SetPosition(const Tile * const tile) {
    if (!tile || tile == this->tile) return;

    if (suspense && !unsuspensed) {
        suspense = false;
        unsuspensed = true;
		changeFocus = true;
    } else {
        lasttile = this->tile;
        cameraMoved = true;
    }

    this->tile = tile;
}

void Camera::Suspend() {
    tile = nullptr;
    lasttile = nullptr;
    suspense = true;
}

// Fill Visible Blocks vector by actual blocks pointers
void Camera::fullRecountVisibleBlocks(const Tile * const tile) {
    if (!tile) {
        Server::log << "Error: fullRecountVisibleBlocks called with nullptr param" << std::endl;
        return;
    }

    // Count coords of first visible tile, and block
    const int firstTileX = tile->X() - Global::FOV / 2 - Global::MIN_PADDING;
    const int firstTileY = tile->Y() - Global::FOV / 2 - Global::MIN_PADDING;
    firstBlockX = firstTileX / Global::BLOCK_SIZE + (firstTileX < 0 ? -1 : 0);
    firstBlockY = firstTileY / Global::BLOCK_SIZE + (firstTileY < 0 ? -1 : 0);

    // Filling our result vector by block pointers
    int y = firstBlockY;
    for (auto &vect : visibleBlocks) {
        int x = firstBlockX;
        for (auto *&block : vect) {
            block = tile->GetMap()->GetBlock(x, y);
            x++;
        }
        y++;
    }

    blockShifted = true;

    for (auto &vect : blocksSync)
        vect = std::vector<bool>(visibleBlocksNum, false);
}

// Commit shift to Visible Blocks vector, saving seen blocks with their sync param
void Camera::refreshVisibleBlocks(const Tile * const tile) {
    if (suspense) {
        Server::log << "Error: refreshVisibleBlocks called by suspensed camera" << std::endl;
        return;
    }
    if (!tile) {
        Server::log << "Error: refreshVisibleBlocks called with nullptr param" << std::endl;
        return;
    }

    const int firstVisibleTileX = firstBlockX * Global::BLOCK_SIZE;
    const int firstVisibleTileY = firstBlockY * Global::BLOCK_SIZE;

    // Check the necessity of blocks shift
    if (tile->X() - firstVisibleTileX < Global::MIN_PADDING + Global::FOV / 2 ||
        tile->Y() - firstVisibleTileY < Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileX + visibleBlocksNum * Global::BLOCK_SIZE - tile->X() < Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileY + visibleBlocksNum * Global::BLOCK_SIZE - tile->Y() < Global::MIN_PADDING + Global::FOV / 2)
    {
        const int firstNewTileX = tile->X() - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewTileY = tile->Y() - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewBlockX = firstNewTileX / Global::BLOCK_SIZE + (firstNewTileX < 0 ? -1 : 0);
        const int firstNewBlockY = firstNewTileY / Global::BLOCK_SIZE + (firstNewTileY < 0 ? -1 : 0);

        const int block_dx = firstNewBlockX - firstBlockX;
        const int block_dy = firstNewBlockY - firstBlockY;

        std::vector<std::vector<bool>> saved(visibleBlocksNum, std::vector<bool>(visibleBlocksNum, false));

        for (uint y = 0; y < visibleBlocksNum; y++)
            for (uint x = 0; x < visibleBlocksNum; x++) {
                if (x - block_dx >= 0 && x - block_dx < visibleBlocksNum &&
                    y - block_dy >= 0 && y - block_dy < visibleBlocksNum &&
                    blocksSync[y][x])
                    saved[y - block_dy][x - block_dx] = true;
            }

        fullRecountVisibleBlocks(tile);
        blocksSync = saved; 
    }
}
