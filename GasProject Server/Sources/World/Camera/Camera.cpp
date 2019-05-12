#include "Camera.hpp"

#include <Server.hpp>
#include <Network/Differences.hpp>
#include <Player.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>
#include <World/Atmos/AtmosCameraOverlay.h>

#include <Shared/Command.hpp>
#include <Shared/Array.hpp>

Camera::Camera(const Tile * const tile) :
    tile(nullptr), lasttile(nullptr), suspense(true),
    changeFocus(false),
    unsuspensed(false), cameraMoved(false)
{
    visibleTilesSide = Global::FOV + 2 * Global::MIN_PADDING;
    visibleTilesHeight = Global::Z_FOV | 1;

    visibleBlocks.resize(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

    blocksSync.resize(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

    SetPosition(tile);

	overlays.push_back(std::make_unique<AtmosCameraOverlay>());
}

void Camera::updateOverlay(sf::Time timeElapsed) {
	ICameraOverlay *overlay;
	if (overlays.size())
		overlay = overlays[0].get();
	else
		return;

	if (overlay->IsShouldBeUpdated(timeElapsed)) {
		std::vector<OverlayInfo> overlayTileData;

		overlayTileData.reserve(visibleTilesSide*visibleTilesSide*visibleTilesHeight);
		for (uint i = 0; i < visibleTilesSide*visibleTilesSide*visibleTilesHeight; i++) {
			overlayTileData.push_back(overlay->GetOverlayInfo(*visibleBlocks[i]));
		}

		auto command = std::make_unique<OverlayUpdateServerCommand>();
		command->overlayInfo = std::move(overlayTileData);
		player->AddCommandToClient(command.release());
	}
}

void Camera::UpdateView(sf::Time timeElapsed) {
    if (unsuspensed && cameraMoved) 
        Server::log << "Logic error: camera unsuspensed and moved at one time" << std::endl;

    int updateOptions = GraphicsUpdateServerCommand::Option::EMPTY;
    auto command = std::make_unique<GraphicsUpdateServerCommand>();

    if (unsuspensed || cameraMoved) {
        if (unsuspensed) fullRecountVisibleBlocks(tile);
        else refreshVisibleBlocks(tile);
        updateOptions |= GraphicsUpdateServerCommand::Option::CAMERA_MOVE;
        command->cameraX = tile->GetPos().x;
        command->cameraY = tile->GetPos().y;
        //command->cameraZ = tile->GetPos().z;
    }

    unsuspensed = cameraMoved = false;

    for (uint i = 0; i < visibleTilesSide*visibleTilesSide*visibleTilesHeight; i++) {
		Tile *block = visibleBlocks[i];
		if (block) {
			if (blocksSync[i]) {
				for (auto &diff : block->GetDifferences()) {
					// Check diff visibility
					if (!diff->CheckVisibility(seeInvisibleAbility)) continue;

					// If client doesn't know about moved object, we need to add it
					if (diff->GetType() == Global::DiffType::RELOCATE) {
						ReplaceDiff *replaceDiff = dynamic_cast<ReplaceDiff *>(diff.get());
						Tile *lastBlock = replaceDiff->lastBlock;
						if (!lastBlock ||
							lastBlock->X() < firstBlockX || lastBlock->X() >= firstBlockX + int(visibleTilesSide) ||
							lastBlock->Y() < firstBlockY || lastBlock->Y() >= firstBlockY + int(visibleTilesSide) ||
							lastBlock->Z() < firstBlockZ || lastBlock->Z() >= firstBlockZ + int(visibleTilesHeight)) 
						{
							command->diffs.push_back(std::make_shared<AddDiff>(*replaceDiff));
						}
					}
					//if (diff->GetType() == Global::DiffType::SHIFT) {
					//    continue;
					//}
					command->diffs.push_back(diff);
				}
			} else {
				command->blocksInfo.push_back(block->GetBlockInfo(seeInvisibleAbility));
				blocksSync[i] = true;
			}
		}
	}

    if (blockShifted) {
        updateOptions |= GraphicsUpdateServerCommand::Option::BLOCKS_SHIFT;
        command->firstBlockX = firstBlockX;
        command->firstBlockY = firstBlockY;
        //command->firstBlockZ = firstBlockZ;
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
        player->AddCommandToClient(command.release());

	updateOverlay(timeElapsed);
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
    firstBlockX = tile->GetPos().x - Global::FOV / 2 - Global::MIN_PADDING;
    firstBlockY = tile->GetPos().y - Global::FOV / 2 - Global::MIN_PADDING;
    firstBlockZ = tile->GetPos().z - Global::Z_FOV / 2;

    // Filling our result vector by block pointers
    for (uint z = 0; z < visibleTilesHeight; z++) {
		for (uint y = 0; y < visibleTilesSide; y++) {
			for (uint x = 0; x < visibleTilesSide; x++) {
				visibleBlocks[uf::flat_index({x,y,z},visibleTilesSide,visibleTilesSide)] = tile->GetMap()->GetTile({firstBlockX+x, firstBlockY+y, firstBlockZ+z});
			}
		}
	}

    blockShifted = true;

    fill(blocksSync.begin(), blocksSync.end(), false);
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
    const int firstVisibleTileZ = firstBlockZ;

    // Check the necessity of blocks shift
    if (int(tile->GetPos().x - firstVisibleTileX) < Global::MIN_PADDING + Global::FOV / 2 ||
        int(tile->GetPos().y - firstVisibleTileY) < Global::MIN_PADDING + Global::FOV / 2 ||
        int(tile->GetPos().z - firstVisibleTileZ) < Global::Z_FOV / 2 ||
        firstVisibleTileX + visibleTilesSide * Global::BLOCK_SIZE - tile->GetPos().x <= Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileY + visibleTilesSide * Global::BLOCK_SIZE - tile->GetPos().y <= Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileZ + visibleTilesHeight - tile->GetPos().z <= Global::Z_FOV / 2)
    {
        const int firstNewBlockX = tile->GetPos().x - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewBlockY = tile->GetPos().y - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewBlockZ = tile->GetPos().z - Global::Z_FOV / 2;

        const int block_dx = firstNewBlockX - firstBlockX;
        const int block_dy = firstNewBlockY - firstBlockY;
        const int block_dz = firstNewBlockZ - firstBlockZ;

        std::vector<bool> saved(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

        for (uint y = 0; y < visibleTilesSide; y++)
            for (uint x = 0; x < visibleTilesSide; x++)
				for (uint z = 0; z < visibleTilesHeight; z++) {
                if (x - block_dx >= 0 && x - block_dx < visibleTilesSide &&
                    y - block_dy >= 0 && y - block_dy < visibleTilesSide &&
                    z - block_dz >= 0 && z - block_dz < visibleTilesHeight &&
                    blocksSync[uf::flat_index({x,y,z},visibleTilesSide,visibleTilesSide)])
                    saved[uf::flat_index({x-block_dx,y-block_dy,z-block_dz},visibleTilesSide,visibleTilesSide)] = true;
				}

        fullRecountVisibleBlocks(tile);
        blocksSync = saved; 
    }
}
