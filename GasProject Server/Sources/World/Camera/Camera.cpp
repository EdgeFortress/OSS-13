#include "Camera.hpp"

#include <plog/Log.h>

#include <IGame.h>
#include <Network/Differences.hpp>
#include <Player.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>
#include <World/Atmos/AtmosCameraOverlay.h>

#include <Shared/Command.hpp>
#include <Shared/Array.hpp>
#include <Shared/Network/Protocol/ServerToClient/Commands.h>

#include <Shared/ErrorHandling.h>

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
}

void Camera::Update(std::chrono::microseconds timeElapsed) {
	if (trackingObject)
		if (tile != trackingObject->GetTile())
			SetPosition(trackingObject->GetTile());
}

void Camera::updateOverlay(std::chrono::microseconds timeElapsed) {
	if (!overlay)
		return;
	if (overlay->IsShouldBeUpdated(timeElapsed)) {
		std::vector<network::protocol::OverlayInfo> overlayTileData;

		overlayTileData.reserve(visibleTilesSide*visibleTilesSide*visibleTilesHeight);
		for (auto &tile: visibleBlocks) {
			if (tile) {
				overlayTileData.push_back(overlay->GetOverlayInfo(*tile));
			}
		}

		auto command = std::make_unique<OverlayUpdateServerCommand>();
		command->overlayInfo = std::move(overlayTileData);
		//player->AddCommandToClient(command.release());
	}
}

bool CheckVisibility(uint id, uint invisibility, uint viewerId, uint visibility) { // TODO: remove this (copied from object)
	return !id                               // viewer is pure camera without object => we can see everything
		|| id == viewerId                    // diff related to viewer itself
		|| !(~(~invisibility | visibility)); // if invisible flag is true, then corresponding visible flag should be true
}

void Camera::UpdateView(std::chrono::microseconds timeElapsed) {
    if (unsuspensed && cameraMoved) 
        LOGE << "Logic error: camera unsuspensed and moved at one time";

	using namespace network::protocol;

    int updateOptions = server::GraphicsUpdateCommand::Option::EMPTY;
    auto command = std::make_unique<server::GraphicsUpdateCommand>();

    if (unsuspensed || cameraMoved) {
        if (unsuspensed) {
			fullRecountVisibleBlocks(tile);
			visibleObjects.clear();
		} else refreshVisibleBlocks(tile);
        updateOptions |= server::GraphicsUpdateCommand::Option::CAMERA_MOVE;
		command->camera = tile->GetPos();
    }

    unsuspensed = cameraMoved = false;

	Object *viewer = player->GetControl()->GetOwner();
	uint viewerId = viewer ? viewer->ID() : 0;

    for (int i = 0; i < visibleTilesSide*visibleTilesSide*visibleTilesHeight; i++) {
		Tile *tile = visibleBlocks[i];
		if (tile) {
			if (blocksSync[i]) {
				for (auto &diffAndInvisibility : tile->GetDifferencesWithInvisibility()) {
					auto generalDiff = diffAndInvisibility.first;
					uint invisibility = diffAndInvisibility.second;

					if (!CheckVisibility(generalDiff->objId, invisibility, viewerId, seeInvisibleAbility))
						continue;

					if (auto *diff = dynamic_cast<network::protocol::AddDiff *>(generalDiff.get())) {
						CHECK(visibleObjects.find(diff->objId) == visibleObjects.end()); // debug
						visibleObjects.insert(diff->objId);
					}
					else if (auto *diff = dynamic_cast<network::protocol::MoveDiff *>(generalDiff.get())) {
						if (visibleObjects.find(diff->objId) == visibleObjects.end()) {
							apos to = tile->GetPos() - rpos(DirectionToVect(diff->direction), 0);
							auto addDiff = std::make_shared<network::protocol::AddDiff>();
							addDiff->objId = diff->objId;
							addDiff->objectInfo = GGame->GetWorld()->GetObject(diff->objId)->GetObjectInfo(); // TODO: Consider races with main thread
							addDiff->coords = to;
							addDiff->invisibility = diff->invisibility;
							command->diffs.push_back(std::move(addDiff));
							visibleObjects.insert(diff->objId);
							continue;
						}
					}
					else if (auto *diff = dynamic_cast<network::protocol::RelocateAwayDiff *>(generalDiff.get())) {
						CHECK(visibleObjects.find(diff->objId) != visibleObjects.end()); // debug
						if (diff->newCoords >= rpos(firstBlockX, firstBlockY, firstBlockZ) &&
							diff->newCoords < rpos(firstBlockX + visibleTilesSide, firstBlockY + visibleTilesSide, firstBlockZ + visibleTilesHeight))
						{
							continue;
						}
						visibleObjects.erase(diff->objId);

						auto removeDiff = std::make_shared<network::protocol::RemoveDiff>();
						removeDiff->objId = diff->objId;
						removeDiff->invisibility = diff->invisibility;

						command->diffs.push_back(std::move(removeDiff));
						continue;

					}
					else if (auto *diff = dynamic_cast<network::protocol::RelocateDiff *>(generalDiff.get())) {
						if (visibleObjects.find(diff->objId) == visibleObjects.end()) {
							auto addDiff = std::make_shared<network::protocol::AddDiff>();
							addDiff->objId = diff->objId;
							addDiff->objectInfo = diff->objectInfo;
							addDiff->coords = diff->newCoords;
							addDiff->invisibility = diff->invisibility;
							command->diffs.push_back(std::move(addDiff));
							visibleObjects.insert(diff->objId);
							continue;
						}
					}
					else if (auto *diff = dynamic_cast<network::protocol::RemoveDiff *>(generalDiff.get())) {
						CHECK(visibleObjects.find(diff->objId) != visibleObjects.end()); // debug
						visibleObjects.erase(diff->objId);
					};

					command->diffs.push_back(std::move(generalDiff));
				}
			} else {
				command->tilesInfo.push_back(tile->GetTileInfo(viewerId, seeInvisibleAbility));
				for (auto &object: tile->Content()) {
					if (!object->CheckVisibility(viewerId, seeInvisibleAbility)) {
						continue;
					}
					visibleObjects.insert(object->ID());
				}
				blocksSync[i] = true;
			}
		}
	}

	std::sort(command->diffs.begin(), command->diffs.end(), [](const sptr<network::protocol::Diff> &a, const sptr<network::protocol::Diff> &b) {
		return a->Id() != a->Id() &&
			(a->Id() == "AddDiff"_crc32 ||
			 a->Id() == "RemoveDiff"_crc32);
	});

	if (blockShifted) {
		updateOptions |= server::GraphicsUpdateCommand::Option::TILES_SHIFT;
		command->firstTile = {firstBlockX, firstBlockY, firstBlockZ};
		blockShifted = false;
	}

	if (command->diffs.size()) {
		updateOptions |= GraphicsUpdateServerCommand::Option::DIFFERENCES;
	}

	if (changeFocus) {
		updateOptions |= server::GraphicsUpdateCommand::Option::NEW_CONTROLLABLE;
		command->controllableId = player->GetControl()->GetOwner()->ID();
		command->controllableSpeed = player->GetControl()->GetSpeed();
		changeFocus = false;
	}

	command->options = server::GraphicsUpdateCommand::Option(updateOptions);
    
	if (updateOptions)
		player->AddCommandToClient(command.release());

	updateOverlay(timeElapsed);
}

void Camera::SetPlayer(Player * const player) { this->player = player; changeFocus = true; }
void Camera::TrackObject(Object *obj) { trackingObject = obj; }

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

void Camera::SetOverlay(uptr<ICameraOverlay> &&overlay) {
	this->overlay = std::forward<uptr<ICameraOverlay>>(overlay);
}

void Camera::ResetOverlay() {
	this->overlay.reset();
	//player->AddCommandToClient(new OverlayResetServerCommand());
}

// Fill Visible Blocks vector by actual blocks pointers
void Camera::fullRecountVisibleBlocks(const Tile * const tile) {
    if (!tile) {
        LOGE << "Error: fullRecountVisibleBlocks called with nullptr param";
        return;
    }

    // Count coords of first visible tile, and block
    firstBlockX = tile->GetPos().x - Global::FOV / 2 - Global::MIN_PADDING;
    firstBlockY = tile->GetPos().y - Global::FOV / 2 - Global::MIN_PADDING;
    firstBlockZ = tile->GetPos().z - Global::Z_FOV / 2;

    // Filling our result vector by block pointers
    for (int z = 0; z < visibleTilesHeight; z++) {
		for (int y = 0; y < visibleTilesSide; y++) {
			for (int x = 0; x < visibleTilesSide; x++) {
				visibleBlocks[flat_index({x,y,z})] = tile->GetMap()->GetTile({firstBlockX+x, firstBlockY+y, firstBlockZ+z});
			}
		}
	}

    blockShifted = true;

    fill(blocksSync.begin(), blocksSync.end(), false);
}

// Commit shift to Visible Blocks vector, saving seen blocks with their sync param
void Camera::refreshVisibleBlocks(const Tile * const tile) {
    if (suspense) {
        LOGE << "Error: refreshVisibleBlocks called by suspensed camera";
        return;
    }
    if (!tile) {
        LOGE << "Error: refreshVisibleBlocks called with nullptr param";
        return;
    }

    const int firstVisibleTileX = firstBlockX;
    const int firstVisibleTileY = firstBlockY;
    const int firstVisibleTileZ = firstBlockZ;

    // Check the necessity of blocks shift
    if (int(tile->GetPos().x - firstVisibleTileX) < Global::MIN_PADDING + Global::FOV / 2 ||
        int(tile->GetPos().y - firstVisibleTileY) < Global::MIN_PADDING + Global::FOV / 2 ||
        int(tile->GetPos().z - firstVisibleTileZ) < Global::Z_FOV / 2 ||
        firstVisibleTileX + visibleTilesSide - tile->GetPos().x <= Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileY + visibleTilesSide - tile->GetPos().y <= Global::MIN_PADDING + Global::FOV / 2 ||
        firstVisibleTileZ + visibleTilesHeight - tile->GetPos().z <= Global::Z_FOV / 2)
    {
        const int firstNewBlockX = tile->GetPos().x - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewBlockY = tile->GetPos().y - Global::FOV / 2 - Global::MIN_PADDING;
        const int firstNewBlockZ = tile->GetPos().z - Global::Z_FOV / 2;

        const int block_dx = firstNewBlockX - firstBlockX;
        const int block_dy = firstNewBlockY - firstBlockY;
        const int block_dz = firstNewBlockZ - firstBlockZ;

        std::vector<bool> saved(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

        for (int y = 0; y < visibleTilesSide; y++)
            for (int x = 0; x < visibleTilesSide; x++)
				for (int z = 0; z < visibleTilesHeight; z++) {
					if (blocksSync[flat_index({x,y,z})]) {
						if (x - block_dx >= 0 && x - block_dx < visibleTilesSide &&
							y - block_dy >= 0 && y - block_dy < visibleTilesSide &&
							z - block_dz >= 0 && z - block_dz < visibleTilesHeight)
						{
							saved[flat_index({x-block_dx,y-block_dy,z-block_dz})] = true;
						} else {
							Tile *block = visibleBlocks[flat_index({x,y,z})];
							if (block) {
								for (auto &object: block->Content()) {
									visibleObjects.erase(object->ID());
								}
							}
						}
					}
				}

        fullRecountVisibleBlocks(tile);
        blocksSync = saved; 
    }
}

int Camera::flat_index(uf::vec3i c) const {
	return uf::flat_index(c, visibleTilesSide, visibleTilesSide);
}
