#include "Camera.hpp"

#include <plog/Log.h>

#include <IGame.h>
#include <Player.hpp>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>
#include <World/Atmos/AtmosCameraOverlay.h>

#include <Shared/Network/Protocol/ServerToClient/Commands.h>

#include <Shared/ErrorHandling.h>

Camera::Camera(const Tile * const tile) :
    tile(nullptr), lasttile(nullptr), suspense(true),
    changeFocus(false),
    unsuspensed(false), cameraMoved(false)
{
	fov = Global::FOV;
	fovZ = Global::Z_FOV;
    visibleTilesSide = fov * 2 + 1 + 2 * Global::MIN_PADDING;
    visibleTilesHeight = fovZ * 2 + 1;

    visibleBlocks.SetSize(visibleTilesSide, visibleTilesSide, visibleTilesHeight);
    visibleBlocks.SetRemovedCallback(std::bind(&Camera::unsee, this, std::placeholders::_1));

    blocksSync.SetSize(visibleTilesSide, visibleTilesSide, visibleTilesHeight);

    SetPosition(tile);
}

void Camera::SetFOV(int _fov) {
	fovBuffer = _fov;
	changeFov = true;
}

void Camera::SetFOVZ(int _fovZ) {
	fovZBuffer = _fovZ;
	changeFov = true;
}

int Camera::GetFOV() {return fov;}
int Camera::GetFOVZ() {return fovZ;}

void Camera::Update(std::chrono::microseconds timeElapsed) {
	updateContextMenu();
	if (trackingObject)
		if (tile != trackingObject->GetTile())
			SetPosition(trackingObject->GetTile());
}

void Camera::updateOverlay(std::chrono::microseconds timeElapsed) {
	if (!overlay)
		return;
	if (overlay->IsShouldBeUpdated(timeElapsed)) {
		auto command = std::make_unique<network::protocol::server::OverlayUpdateCommand>();

		command->overlayInfo.reserve(visibleTilesSide*visibleTilesSide*visibleTilesHeight);
		for (auto &tile: visibleBlocks.Get()) {
			if (tile) {
				command->overlayInfo.push_back(overlay->GetOverlayInfo(*tile));
			}
		}

		player->AddCommandToClient(command.release());
	}
}

void fillNode(Object *obj, network::protocol::ContextMenuNode &node, std::vector<ContextMenuNodeCache> &cache) {
	node.title = obj->GetName();
	std::vector<std::string> sendedVerbs;
	for (auto &[key, verb] : obj->GetVerbs()) {
		node.verbs.push_back(key);
		sendedVerbs.push_back(key);
	}
	cache.push_back({ obj, std::move(sendedVerbs) });
}

void addTileObjectsToContextMenu(Tile *tile, network::protocol::server::UpdateContextMenuCommand &command, std::vector<ContextMenuNodeCache> &cache) {
	cache.clear();

	for (auto &obj : tile->Content()) {
		auto node = std::make_unique<network::protocol::ContextMenuNode>();
		fillNode(obj, *node, cache);
		command.data.nodes.push_back(std::move(node));
	}
}

void Camera::updateContextMenu() {
	if (!askedUpdateContextMenu)
		return;
	askedUpdateContextMenu = false;
	
	EXPECT(contextMenuTileCoords >= uf::vec3i(0) && contextMenuTileCoords < uf::vec3i(visibleTilesSide, visibleTilesSide, visibleTilesHeight));
	contextMenuTile = visibleBlocks.At(contextMenuTileCoords);

	auto command = std::make_unique<network::protocol::server::UpdateContextMenuCommand>();
	addTileObjectsToContextMenu(contextMenuTile, *command, contextMenuCache);

	EXPECT(player);
	player->AddCommandToClient(command.release());
}

void Camera::UpdateView(std::chrono::microseconds timeElapsed) {
    if (unsuspensed && cameraMoved) 
        LOGE << "Logic error: camera unsuspensed and moved at one time";

	using namespace network::protocol;

    int updateOptions = server::GraphicsUpdateCommand::Option::EMPTY;
    auto command = std::make_unique<server::GraphicsUpdateCommand>();

	if (changeFov) {
		updateFOV();
		updateOptions |= server::GraphicsUpdateCommand::Option::NEW_FOV;
		command->fov = fov;
		command->fovZ = fovZ;
		changeFov = false;
	}

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

	std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> differencesWithObjects;

	for (int i = 0; i < visibleTilesSide * visibleTilesSide * visibleTilesHeight; i++) {
		Tile *tile = visibleBlocks.Get()[i];
		if (tile) {
			if (blocksSync.Get()[i]) {
				// Collect differences
				differencesWithObjects.insert(differencesWithObjects.end(), tile->GetDifferencesWithObject().begin(), tile->GetDifferencesWithObject().end());
			} else {
				command->tilesInfo.push_back(tile->GetTileInfo(viewerId, seeInvisibleAbility));
				for (auto &object: tile->Content()) {
					if (!object->CheckVisibility(viewerId, seeInvisibleAbility)) {
						continue;
					}
					visibleObjects.insert(object->ID());
				}
				blocksSync.Get()[i] = true;
			}
		}
	}

	// Sort differences
	std::sort(differencesWithObjects.begin(), differencesWithObjects.end(), 
			[](const std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>> &a, const std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>> &b) {
				return a.first->GetDiffId() < b.first->GetDiffId();
			}
	);

	// Process differences
	for (auto &diffAndObject : differencesWithObjects) {
		auto &generalDiff = diffAndObject.first;
		auto &object = diffAndObject.second;

		if (!object->CheckVisibility(viewerId, seeInvisibleAbility))
			continue;

		if (auto *diff = dynamic_cast<network::protocol::AddDiff *>(generalDiff.get())) {
			CHECK(visibleObjects.find(diff->objId) == visibleObjects.end()); // debug
			visibleObjects.insert(diff->objId);
		} else if (auto *diff = dynamic_cast<network::protocol::MoveDiff *>(generalDiff.get())) {
			if (visibleObjects.find(diff->objId) == visibleObjects.end()) {
				apos to = apos(object->GetPosition() + DirectionToVect(diff->direction), 0);
				auto addDiff = std::make_shared<network::protocol::AddDiff>();
				addDiff->objId = diff->objId;
				addDiff->objectInfo = object->GetObjectInfo();
				addDiff->coords = to;
				command->diffs.push_back(std::move(addDiff));
				visibleObjects.insert(diff->objId);
				continue;
			}
		} else if (auto *diff = dynamic_cast<network::protocol::RelocateAwayDiff *>(generalDiff.get())) {
			CHECK(visibleObjects.find(diff->objId) != visibleObjects.end()); // debug
			if (diff->newCoords >= rpos(firstBlockX, firstBlockY, firstBlockZ) &&
				diff->newCoords < rpos(firstBlockX + visibleTilesSide, firstBlockY + visibleTilesSide, firstBlockZ + visibleTilesHeight))
			{
				continue;
			}
			visibleObjects.erase(diff->objId);

			auto removeDiff = std::make_shared<network::protocol::RemoveDiff>();
			removeDiff->objId = diff->objId;

			command->diffs.push_back(std::move(removeDiff));
			continue;

		} else if (auto *diff = dynamic_cast<network::protocol::RelocateDiff *>(generalDiff.get())) {
			if (visibleObjects.find(diff->objId) == visibleObjects.end()) {
				auto addDiff = std::make_shared<network::protocol::AddDiff>();
				addDiff->objId = diff->objId;
				addDiff->objectInfo = object->GetObjectInfo();
				addDiff->coords = diff->newCoords;
				command->diffs.push_back(std::move(addDiff));
				visibleObjects.insert(diff->objId);
				continue;
			}
		} else if (auto *diff = dynamic_cast<network::protocol::RemoveDiff *>(generalDiff.get())) {
			CHECK(visibleObjects.find(diff->objId) != visibleObjects.end()); // debug
			visibleObjects.erase(diff->objId);
		};

		command->diffs.push_back(std::move(generalDiff));
	}

	if (blockShifted) {
		updateOptions |= server::GraphicsUpdateCommand::Option::TILES_SHIFT;
		command->firstTile = {firstBlockX, firstBlockY, firstBlockZ};
		blockShifted = false;
	}

	if (command->diffs.size()) {
		updateOptions |= server::GraphicsUpdateCommand::Option::DIFFERENCES;
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

void Camera::AskUpdateContextMenu(uf::vec3i tile) {
	askedUpdateContextMenu = true;
	contextMenuTileCoords = tile;
}

void Camera::ClickContextMenu(uint8_t node, uint8_t verb) {
	contextMenuCache[node].obj->CallVerb(player, contextMenuCache[node].verbs[verb]);
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
	auto command = std::make_unique<network::protocol::server::OverlayResetCommand>();
	player->AddCommandToClient(command.release());
}

// Fill Visible Blocks vector by actual blocks pointers
void Camera::fullRecountVisibleBlocks(const Tile * const tile) {
    if (!tile) {
        LOGE << "Error: fullRecountVisibleBlocks called with nullptr param";
        return;
    }

    // Count coords of first visible tile, and block
    firstBlockX = tile->GetPos().x - fov - Global::MIN_PADDING;
    firstBlockY = tile->GetPos().y - fov - Global::MIN_PADDING;
    firstBlockZ = tile->GetPos().z - fovZ;

    // Filling our result vector by block pointers
    for (int z = 0; z < visibleTilesHeight; z++) {
		for (int y = 0; y < visibleTilesSide; y++) {
			for (int x = 0; x < visibleTilesSide; x++) {
				visibleBlocks.At(x,y,z) = tile->GetMap()->GetTile({firstBlockX+x, firstBlockY+y, firstBlockZ+z});
			}
		}
	}

    blockShifted = true;

    fill(blocksSync.Get().begin(), blocksSync.Get().end(), false);
}

void Camera::fillEmptyVisibleBlocks() {
	for (int z = 0; z < visibleTilesHeight; z++)
	for (int y = 0; y < visibleTilesSide; y++)
	for (int x = 0; x < visibleTilesSide; x++) {
		if(visibleBlocks.At(x,y,z) != nullptr) {
			continue;
		}
		visibleBlocks.At(x,y,z) = tile->GetMap()->GetTile({firstBlockX + x, firstBlockY + y, firstBlockZ + z});
	}
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
    if (int(tile->GetPos().x - firstVisibleTileX) < Global::MIN_PADDING + fov ||
        int(tile->GetPos().y - firstVisibleTileY) < Global::MIN_PADDING + fov ||
        int(tile->GetPos().z - firstVisibleTileZ) < fovZ ||
        firstVisibleTileX + visibleTilesSide - tile->GetPos().x <= Global::MIN_PADDING + fov ||
        firstVisibleTileY + visibleTilesSide - tile->GetPos().y <= Global::MIN_PADDING + fov ||
        firstVisibleTileZ + visibleTilesHeight - tile->GetPos().z <= fovZ)
    {
        const int firstNewBlockX = tile->GetPos().x - fov - Global::MIN_PADDING;
        const int firstNewBlockY = tile->GetPos().y - fov - Global::MIN_PADDING;
        const int firstNewBlockZ = tile->GetPos().z - fovZ;

        const int block_dx = firstNewBlockX - firstBlockX;
        const int block_dy = firstNewBlockY - firstBlockY;
        const int block_dz = firstNewBlockZ - firstBlockZ;

		GridTransformation transformation;
		transformation.originDelta = {block_dx, block_dy, block_dz};

		visibleBlocks.Transform(transformation);
		blocksSync.Transform(transformation);

		fillEmptyVisibleBlocks();

		blockShifted = true;
		firstBlockX = firstNewBlockX;
		firstBlockY = firstNewBlockY;
		firstBlockZ = firstNewBlockZ;
    }
}


void Camera::updateFOV() {
	int newSide = fovBuffer * 2 + 1 + 2 * Global::MIN_PADDING;
	int newHeight = fovZBuffer * 2 + 1;
	int diff = (visibleTilesSide - newSide) / 2;
	int diff_z = (visibleTilesHeight - newHeight) / 2;

	GridTransformation transformation;
	transformation.originDelta = {diff, diff, diff_z};
	transformation.sizeDelta = {newSide - visibleTilesSide, newSide - visibleTilesSide, newHeight - visibleTilesHeight};

	visibleBlocks.Transform(transformation);
	blocksSync.Transform(transformation);

	firstBlockX += diff;
	firstBlockY += diff;
	firstBlockZ += diff_z;
	visibleTilesSide = newSide;
	visibleTilesHeight = newHeight;
	changeFov = false;
	fov = fovBuffer;
	fovZ = fovZBuffer;
}

void Camera::unsee(uf::vec3u pos) {
	Tile *block = visibleBlocks.At(pos);
	if (block) {
		for (auto &object: block->Content()) {
			visibleObjects.erase(object->ID());
		}
	}
}
