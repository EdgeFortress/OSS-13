#include "Camera.hpp"

#include <Shared/Network/Protocol/ServerToClient/Commands.h>
#include <Shared/ErrorHandling.h>

#include <IGame.h>
#include <Player/Player.h>
#include <World/World.hpp>
#include <World/Map.hpp>
#include <World/Objects/Control.hpp>
#include <World/Subsystems/Atmos/AtmosCameraOverlay.h>

Camera::Camera(const Tile * const tile)
{
	fov = Global::tilegrid::FOV;
	fovZ = Global::tilegrid::Z_FOV;
	int side = getVisibleAreaSide();
	int height = getVisibleAreaHeight();

	visibleTiles.SetSize(side, side, height);
	visibleTiles.SetRemovedCallback(std::bind(&Camera::unseeTile, this, std::placeholders::_1));

	SetPosition(tile);
}

void Camera::Update(std::chrono::microseconds /*timeElapsed*/) {
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

		command->overlayInfo.reserve(getVisibleAreaSide() * getVisibleAreaSide() * getVisibleAreaHeight());
		for (auto &[tile, _]: visibleTiles.Items()) {
			if (tile) {
				command->overlayInfo.push_back(overlay->GetOverlayInfo(*tile));
			}
		}

		player->AddCommandToClient(command.release());
	}
}

static void fillNode(Object *obj, network::protocol::ContextMenuNode &node, std::vector<ContextMenuNodeCache> &cache) {
	node.title = obj->GetName();
	std::vector<std::string> sendedVerbs;
	for (auto &[key, verb] : obj->GetVerbs()) {
		node.verbs.push_back(key);
		sendedVerbs.push_back(key);
	}
	cache.push_back({ obj, std::move(sendedVerbs) });
}

static void addTileObjectsToContextMenu(Tile *tile, network::protocol::server::UpdateContextMenuCommand &command, std::vector<ContextMenuNodeCache> &cache) {
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
	
	EXPECT(contextMenuTileCoords >= uf::vec3i(0) && contextMenuTileCoords < getVisibleAreaDimensions());
	contextMenuTile = visibleTiles.At(contextMenuTileCoords).first;

	auto command = std::make_unique<network::protocol::server::UpdateContextMenuCommand>();
	addTileObjectsToContextMenu(contextMenuTile, *command, contextMenuCache);

	EXPECT(player);
	player->AddCommandToClient(command.release());
}

void Camera::UpdateView(std::chrono::microseconds timeElapsed) {
	if (unsuspended && cameraMoved) {
		LOGE << "Logic error: camera unsuspended and moved at one time";
	}

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

	if (unsuspended || cameraMoved) {
		if (unsuspended) {
			fullRecountVisibleBlocks();
		} else {
			refreshVisibleBlocks();
		}
		updateOptions |= server::GraphicsUpdateCommand::Option::CAMERA_MOVE | server::GraphicsUpdateCommand::Option::TILES_SHIFT;
		command->camera = tile->GetPos();
		command->firstTile = getFirstTilePos();
	}

	unsuspended = cameraMoved = false;
	lastTile = nullptr;

	Object *viewer = player->GetControl()->GetOwner();
	uint viewerId = viewer ? viewer->ID() : 0;

	std::vector<std::pair<std::shared_ptr<network::protocol::Diff>, sptr<Object>>> differencesWithObjects;

	for (auto &[tile,sync]: visibleTiles.Items()) {
		if (tile) {
			if (sync) {
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
				sync = true;
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
				apos to = apos(object->GetPosition() + DirectionToVect(diff->direction).xy(), 0);
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
			if (diff->newCoords >= getFirstTilePos() &&
				diff->newCoords < getFirstTilePos() + getVisibleAreaDimensions())
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

void Camera::SetPlayer(Player * const player) {
	this->player = player;
	changeFocus = true;
}

void Camera::TrackObject(Object *obj) {
	trackingObject = obj;
}

void Camera::SetPosition(const Tile * const newTile) {
	if (!newTile || newTile == tile) return;

	if (suspense && !unsuspended) {
		suspense = false;
		unsuspended = true;
		changeFocus = true;
	} else {
		if(!cameraMoved) {
			lastTile = tile;
		}
		cameraMoved = lastTile != newTile;
	}

	tile = newTile;
}

uf::vec3i Camera::ViewCoordsToWorldCoords(uf::vec3i viewCoords)
{
	static const int SIDE_CENTER = getVisibleAreaSide() / 2;
	static const uf::vec3i CAMERA_POS_ON_SCREEN = { SIDE_CENTER, SIDE_CENTER, 0 };
	return GetPosition()->GetPos() - CAMERA_POS_ON_SCREEN + viewCoords;
}

void Camera::Suspend() {
	tile = nullptr;
	lastTile = nullptr;
	suspense = true;
}

void Camera::SetFOV(int _fov) {
	fov = _fov;
	changeFov = true;
}

void Camera::SetFOVZ(int _fovZ) {
	fovZ = _fovZ;
	changeFov = true;
}

int Camera::GetFOV()  {return fov;}
int Camera::GetFOVZ() {return fovZ;}

void Camera::SetOverlay(uptr<ICameraOverlay> &&overlay) {
	this->overlay = std::forward<uptr<ICameraOverlay>>(overlay);
}

void Camera::ResetOverlay() {
	this->overlay.reset();
	auto command = std::make_unique<network::protocol::server::OverlayResetCommand>();
	player->AddCommandToClient(command.release());
}

int Camera::getVisibleAreaSide() {
	return fov * 2 + 1 + 2 * Global::tilegrid::MIN_PADDING;
}

int Camera::getVisibleAreaHeight() {
	return fovZ * 2 + 1;
}

uf::vec3i Camera::getVisibleAreaDimensions() {
	return visibleTiles.GetSize();
}

uf::vec3i Camera::getFirstTilePos() {
	return tile->GetPos() - uf::vec3i(fov + Global::tilegrid::MIN_PADDING, fov + Global::tilegrid::MIN_PADDING, fovZ);
}

void Camera::fullRecountVisibleBlocks() {
	for (auto &&[pair, pos] : visibleTiles) {
		pair = {tile->GetMap()->GetTile(getFirstTilePos() + pos), false};
	}
	visibleObjects.clear();
}

void Camera::fillEmptyVisibleBlocks() {
	for (auto &&[pair, pos] : visibleTiles) {
		if(pair.first != nullptr) {
			continue;
		}
		pair.first = tile->GetMap()->GetTile(getFirstTilePos() + pos);
	}
}

void Camera::refreshVisibleBlocks() {
	if (suspense) {
		LOGE << "Error: refreshVisibleBlocks called by suspended camera";
		return;
	}

	if (lastTile) {
		uf::GridTransformation transformation;
		transformation.originDelta = tile->GetPos() - lastTile->GetPos();

		visibleTiles.Transform(transformation);
	}
	fillEmptyVisibleBlocks();
}

void Camera::updateFOV() {
	int diff = (visibleTiles.GetSize().x - Global::tilegrid::MIN_PADDING) / 2 - fov;
	int diff_z = visibleTiles.GetSize().z / 2 - fovZ;

	uf::GridTransformation transformation;
	transformation.originDelta = {diff, diff, diff_z};
	transformation.sizeDelta = -(transformation.originDelta * 2);

	visibleTiles.Transform(transformation);

	changeFov = false;
}

void Camera::unseeTile(uf::vec3u pos) {
	Tile *block = visibleTiles.At(pos).first;
	if (block) {
		for (auto &object: block->Content()) {
			visibleObjects.erase(object->ID());
		}
	}
}
