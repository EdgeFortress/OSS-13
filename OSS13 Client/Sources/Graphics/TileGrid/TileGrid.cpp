#include "TileGrid.hpp"

#include <map>
#include <SFML/Graphics.hpp>

#include <Shared/Global.hpp>
#include <Shared/IFaces/IConfig.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

#include <Client.hpp>
#include <ResourceManager.hpp>
#include <Network/Connection.h>
#include <Graphics/Window.hpp>
#include <Graphics/UI/UI.hpp>

#include "Object.hpp"
#include "Tile.hpp"

#include <imgui.h>

using namespace network::protocol;

TileGrid::TileGrid() :
	tileSize(32), overlayToggled(false),
	controllable(nullptr), controllableSpeed(0), cursorPosition({-1, -1}),
	underCursorObject(nullptr), dropButtonPressed(false),
	buildButtonPressed(false), ghostButtonPressed(false)
{
	//
	// Count num of visible blocks by tile padding and FOV
	//
	// num * size - (2 * pad + fov) >= size
	// num >= (size + 2 * pad + fov) / size
	// We need minimal num, so add 1 if not divided
	fov = Global::FOV;
	fovZ = Global::Z_FOV;
	visibleTilesSide = (fov*2+1) + 2 * Global::MIN_PADDING;
	visibleTilesHeight = (fovZ*2+1);

	// Allocate memory for blocks
	blocks.SetSize(visibleTilesSide, visibleTilesSide, visibleTilesHeight);
	blocks.SetMovedCallback(std::bind(&TileGrid::updatePos, this, std::placeholders::_1, std::placeholders::_2));

	CustomWidget::SetSize(uf::vec2i(fov * 2 + 1) * GetTileSize());

	canBeActive = true;

	layersBuffer.resize(101);

	movementPredictionDisabled = CC::Get()->RM.Config()->GetBool("Debug.MovementPredictionDisabled");

	controlUI = new ControlUI();
	AddItem(controlUI, {0, 0});
}

void TileGrid::drawContainer() const {
    underCursorObject = nullptr;
	buffer.clear();
	const int border = fov + Global::MIN_PADDING;

    // Firstly, fill layers buffer by objects
    uf::vec2i tilePos; // tiles positions relative to camera
    for (tilePos.y = -border; tilePos.y <= border; tilePos.y++)
        for (tilePos.x = -border; tilePos.x <= border; tilePos.x++) {
            Tile *tile = GetTileAbs(cameraPos + rpos(tilePos, cameraZ));
            if (tile) {
                tile->Draw(&buffer, (tilePos + uf::vec2i(fov) - shift) * tileSize);
                for (auto &obj : tile->content) {
                    uint layer = obj->GetLayer();
                    if (layer) layersBuffer[obj->GetLayer()].push_back(obj); // if layer is 0, then object will not be drawn
                }
            }
        }

    // Secondly, draw layers
	for (auto &layerObjects : layersBuffer) {
        for (auto &object: layerObjects) {
            Tile *tile = object->GetTile();
            if ((rpos(tile->GetRelPos()) - rpos(cameraRelPos)).z != cameraZ) {
				continue;
			}
            uf::vec2i pixel = (uf::vec2i(fov) + rpos(tile->GetRelPos() - cameraRelPos).xy() - shift) * tileSize;
            object->Draw(&buffer, pixel);
            if (cursorPosition >= pixel && cursorPosition < pixel + uf::vec2i(tileSize)) {
                if (!object->PixelTransparent(cursorPosition - pixel))
                    underCursorObject = object;
            }
	    }
        layerObjects.clear(); // clear buffer
    }

	// Thirdly, draw overlay
	
	if (overlayToggled) {
		for (tilePos.y = -border; tilePos.y <= border; tilePos.y++)
			for (tilePos.x = -border; tilePos.x <= border; tilePos.x++) {
				Tile *tile = GetTileAbs(cameraPos + rpos(tilePos, cameraZ));
				if (tile) {
					tile->DrawOverlay(&buffer, (tilePos + uf::vec2i(fov) - shift) * tileSize);
				}
			}
	}

	buffer.display();
}

void TileGrid::SetSize(const uf::vec2i &size) {
	CustomWidget::SetScale(size / buffer.getSize().x);
	controlUI->AdjustSize(size);
}

bool TileGrid::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	if (Container::OnMouseButtonPressed(button, position))
		return true;

	switch (button) {
		case sf::Mouse::Left: {
			if (underCursorObject) {
				objectClicked = true;
				return true;
			}
			break;
		}
		case sf::Mouse::Right: {
			if (underCursorTile) {
				rmbClicked = true;
				return true;
			}
			break;
		}
		default:
			break;
	}
	return false;
}

Tile *TileGrid::countTileUnderCursor(uf::vec2i mousePosition) {
	auto coords = mousePosition / tileSize - uf::vec2i(shift) + uf::vec2i(Global::MIN_PADDING);
	return GetTileRel(uf::vec3i(coords, cameraRelPos.z));
}

bool TileGrid::OnMouseMoved(uf::vec2i position) {
	cursorPosition = position;
	cursorPosition -= GetAbsolutePosition();
	cursorPosition = uf::vec2f(cursorPosition) / GetScale().x;
	if (cursorPosition <= GetSize()) {
		underCursorTile = countTileUnderCursor(cursorPosition);
		return true;
	} else {
		cursorPosition = { -1, -1 };
		underCursorTile = nullptr;
		return false;
	}
}

bool TileGrid::OnMouseLeft() {
	cursorPosition = { -1, -1 };
	return true;
}

bool TileGrid::OnMouseWheelScrolled(float delta, uf::vec2i /*position*/) {
	int newCameraZ = cameraZ + static_cast<int>(delta) % 2;
	if (GetTileRel(cameraRelPos + rpos(0, 0, newCameraZ))) {
		cameraZ = newCameraZ;
	}
	return true;
}

bool TileGrid::OnKeyPressed(sf::Event::KeyEvent keyEvent) {
	if (keyEvent.control) {
		switch (keyEvent.code) {
			case sf::Keyboard::D:
				dropButtonPressed = true;
				return true;
			default:
				break;
		}
	}
	switch (keyEvent.code) {
		case sf::Keyboard::Up:
		case sf::Keyboard::W:
			if (keyEvent.shift) {
				moveZCommand = 1;
			} else {
				moveCommand.y = -1;
			}
			return true;
		case sf::Keyboard::Down:
		case sf::Keyboard::S:
			if (keyEvent.shift) {
				moveZCommand = -1;
			} else {
				moveCommand.y = 1;
			}
			return true;
		case sf::Keyboard::Right:
		case sf::Keyboard::D:
			moveCommand.x = 1;
			return true;
		case sf::Keyboard::Left:
		case sf::Keyboard::A:
			moveCommand.x = -1;
			return true;
		case sf::Keyboard::B:
			buildButtonPressed = true;
			return true;
		case sf::Keyboard::G:
			ghostButtonPressed = true;
			return true;
		default:
			break;
	}
	return false;
}

void MovementPrediction(Object *controllable, uf::vec2i moveCommand) {
	EXPECT(controllable);

	Tile *lastTile = controllable->GetTile();
	EXPECT(lastTile);

	uf::vec2i moveIntent = controllable->GetMoveIntent();
	if (moveCommand.x) moveIntent.x = moveCommand.x;
	if (moveCommand.y) moveIntent.y = moveCommand.y;

	Tile *newTileX = lastTile->GetTileGrid()->GetTileRel({ lastTile->GetRelPos().x + moveIntent.x, lastTile->GetRelPos().y, lastTile->GetRelPos().z });
	uf::Direction xDirection = newTileX ? uf::VectToDirection(newTileX->GetRelPos() - lastTile->GetRelPos()) : uf::Direction::NONE;

	Tile *newTileY = lastTile->GetTileGrid()->GetTileRel({ lastTile->GetRelPos().x, lastTile->GetRelPos().y + moveIntent.y, lastTile->GetRelPos().z });
	uf::Direction yDirection = newTileY ? uf::VectToDirection(newTileY->GetRelPos() - lastTile->GetRelPos()) : uf::Direction::NONE;

	Tile *newTileDiag = lastTile->GetTileGrid()->GetTileRel(lastTile->GetRelPos() + rpos(moveIntent, 0));

	if (controllable->IsDense()) {
		auto moveDirection = uf::VectToDirection(moveIntent);

		if (lastTile->IsBlocked(uf::DirectionSet({ moveDirection }))) { // exit from current tile
			moveIntent = controllable->GetMoveIntent();
		} else {
			if (!newTileDiag || newTileDiag->IsBlocked(uf::DirectionSet({ uf::InvertDirection(moveDirection), uf::Direction::CENTER }))) {
				return;
			} else {
				if (!newTileX || (newTileX != lastTile && newTileX->IsBlocked(uf::DirectionSet({ uf::InvertDirection(xDirection), yDirection, uf::Direction::CENTER })))) return;
				if (!newTileY || (newTileY != lastTile && newTileY->IsBlocked(uf::DirectionSet({ uf::InvertDirection(yDirection), xDirection, uf::Direction::CENTER })))) return;
			}
		}
	}

	controllable->SetMoveIntent(moveIntent, false);
}

void TileGrid::Update(sf::Time timeElapsed) {
	Container::Update(timeElapsed);

    if (actionSendPause != sf::Time::Zero) {
        actionSendPause -= timeElapsed;
        if (actionSendPause < sf::Time::Zero) actionSendPause = sf::Time::Zero;
    }

    if (actionSendPause == sf::Time::Zero) {
		if (stun == sf::Time::Zero && !moveCommand.isZero()) {
			auto *p = new client::MoveCommand();
			p->direction = uf::VectToDirection(moveCommand);
			Connection::commandQueue.Push(p);

			controllable->SetDirection(uf::VectToDirection(moveCommand));

			if (!movementPredictionDisabled)
				MovementPrediction(controllable, moveCommand);
		}
		moveCommand = sf::Vector2i();

		if (stun == sf::Time::Zero && moveZCommand) {
			auto *p = new client::MoveZCommand();
			p->up = moveZCommand > 0;
			Connection::commandQueue.Push(p);
		}
		moveZCommand = 0;

        if (stun == sf::Time::Zero && objectClicked && underCursorObject) {
			auto *p = new client::ClickObjectCommand();
			p->id = underCursorObject->GetID();
			Connection::commandQueue.Push(p);
		}

		if (rmbClicked) {
			rmbClicked = false;
			dynamic_cast<GameProcessUI *>(CC::Get()->GetUI()->GetCurrentUIModule())->OpenContextMenu();
			auto command = std::make_unique<network::protocol::client::ContextMenuUpdateCommand>();
			command->tileCoords = underCursorTile->GetRelPos();
			Connection::commandQueue.Push(command.release());
		}

		if (stun == sf::Time::Zero && dropButtonPressed) {
			auto *p = new client::CallVerbCommand();
			p->verb = "creature.drop";
			Connection::commandQueue.Push(p);
		}

		if (ghostButtonPressed) { // TODO: implement hotkeys
			auto *p = new client::CallVerbCommand();
			p->verb = "player.ghost";
			Connection::commandQueue.Push(p);
		}

		objectClicked = false;
		dropButtonPressed = false;
		buildButtonPressed = false;
		ghostButtonPressed = false;

        actionSendPause = ACTION_TIMEOUT;
    }

    for (auto iter = objects.begin(); iter != objects.end();) {
        Object *obj = iter->second.get();
        
        if (!obj->GetTile()) {
            if (underCursorObject == obj) underCursorObject = nullptr;
            iter = objects.erase(iter);
        }
        else {
            obj->Update(timeElapsed);
            iter++;
        }
    }

	for (auto &block : blocks.Items()) {
		if (block) block->Update(timeElapsed);
	}
    
	if (controllable) shift = controllable->GetShift();

	if (stun > timeElapsed)
		stun -= timeElapsed;
	else
		stun = sf::Time::Zero;
}

void TileGrid::AddObject(Object *object) {
	if (!object) {
		throw std::exception(); // "Unexpected!"
		return;
	}
    objects[object->GetID()] = uptr<Object>(object);
    object->Resize(tileSize);
}

void TileGrid::RemoveObject(uint id) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        if (obj->GetTile()) obj->GetTile()->RemoveObject(obj);
		return;
    }

    LOGE << "Error: object with id " << id << " doesn't exist (TileGrid::RemoveObject)";
}

void TileGrid::AmendObjectChanges(network::protocol::FieldsDiff &&diff) {
	auto iter = objects.find(diff.objId);
	EXPECT(iter != objects.end());
	auto &obj = iter->second;
	obj->AmendChanges(std::forward<uf::SyncableChanges>(diff.fieldsChanges));
}

void TileGrid::RelocateObject(uint id, apos toVec, int toObjectNum) {
    Tile *tile = GetTileAbs(toVec);
    if (!tile) {
        LOGE << "Wrong tile absolute coords " << toVec.toString();
        return;
    }

    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        tile->AddObject(obj, toObjectNum);
        obj->ResetShiftingState();
        return;
    }

    LOGE << "Wrong object ID: " << id;
}

void TileGrid::SetMoveIntentObject(uint id, uf::Direction direction) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        uf::vec2i dir = uf::DirectionToVect(direction);
        obj->SetMoveIntent(dir, true);
        return;
    }
    LOGE << "Try to set MoveIntent to unknown object (id: " << id << ")(TileGrid::SetMoveIntentObject)" << std::endl;
}

void TileGrid::MoveObject(uint id, uf::Direction direction, float speed) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        uf::vec2i dir = uf::DirectionToVect(direction);

        Tile *lastTile = obj->GetTile();
        if (!lastTile) {
            LOGE << "Move of unplaced object" << std::endl;
            return;
        }
        Tile *tile = GetTileRel(lastTile->GetRelPos() + rpos(dir, 0));
        if (!tile) {
            LOGE << "Move to unknown tile" << std::endl;
            return;
        }

        //obj->SetShifting(direction, speed);
		obj->SetMoveSpeed(speed);
        tile->AddObject(obj, 0);
        obj->ReverseShifting(direction);
        if (obj == controllable) shift = controllable->GetShift();
        
        return;
    }
	LOGE << "Move of unknown object (id: " << id << ")(TileGrid::MoveObject)" << std::endl;
}

void TileGrid::UpdateObjectIcons(uint id, const std::vector<uint32_t> &icons) {
	auto iter = objects.find(id);
	if (iter == objects.end())
		return;

	Object *obj = iter->second.get();
	obj->ClearSprites();

	for (auto &icon : icons)
		obj->AddSprite(icon);

	obj->Resize(tileSize);
}

void TileGrid::PlayAnimation(uint id, uint animation_id) {
    auto iter = objects.find(id);
    if (iter != objects.end()) {
        Object *obj = iter->second.get();
        obj->PlayAnimation(animation_id);
        obj->Resize(tileSize);
    }
}

void TileGrid::ChangeObjectDirection(uint id, uf::Direction direction) {
    auto iter = objects.find(id);
    if (iter != objects.end()) {
        Object *obj = iter->second.get();
        obj->SetDirection(direction);
    }
}

void TileGrid::Stunned(uint id, sf::Time duration) {
	if (id == controllable->GetID())
		stun = duration;
}

void TileGrid::ShiftBlocks(apos newFirst) {
	uf::GridTransformation transformation;
	transformation.originDelta = newFirst - firstTile;
	blocks.Transform(transformation);
	firstTile = newFirst;
}

void TileGrid::SetCameraPosition(apos pos) {
    cameraPos = pos;
    cameraRelPos = pos - firstTile;
    while (cameraZ && !GetTileRel(cameraRelPos+rpos(0,0,cameraZ))) {
		cameraZ -= cameraZ > 0 ? 1 : -1;
	}
}

void TileGrid::SetBlock(apos pos, std::shared_ptr<Tile> tile) {
    blocks.At(pos - firstTile) = tile;
    tile->relPos = pos - firstTile;
}

void TileGrid::UpdateControlUI(const std::vector<network::protocol::ControlUIData> &elements, bool clear) {
	if (clear)
		controlUI->Clear();
	for (auto &element: elements) {
		controlUI->UpdateElement(element);
	}
}

void TileGrid::SetControllable(uint id, float speed) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        controllable = iter->second.get();
        controllableSpeed = speed;
        return;
    }
	LOGE << "New controllable wasn't founded" << std::endl;
}

void TileGrid::SetFOV(int fov, int fovZ) {
	int newSide = fov * 2 + 1 + 2 * Global::MIN_PADDING;
	int newHeight = fovZ * 2 + 1;
	int diff = (visibleTilesSide - newSide) / 2;
	int diff_z = (visibleTilesHeight - newHeight) / 2;

	uf::GridTransformation transformation;
	transformation.originDelta = {diff, diff, diff_z};
	transformation.sizeDelta = {
			newSide - static_cast<int>(visibleTilesSide),
			newSide - static_cast<int>(visibleTilesSide),
			newHeight - static_cast<int>(visibleTilesHeight)
	};

	blocks.Transform(transformation);

	visibleTilesSide = newSide;
	visibleTilesHeight = newHeight;
	firstTile += uf::vec3i(diff, diff, diff_z);
	cameraRelPos = cameraPos - firstTile;
	this->fov = fov;
	this->fovZ = fovZ;

	auto window = CC::Get()->GetWindow();
	window->GetUI()->GetCurrentUIModule()->Resize(window->GetWidth(), window->GetHeight());
}

int TileGrid::GetFOV() {return fov;}
int TileGrid::GetFOVZ() {return fovZ;}

void TileGrid::UpdateOverlay(std::vector<network::protocol::OverlayInfo> &overlayInfo) {
	overlayToggled = true;
	auto tileOverlayInfo = overlayInfo.begin();
	for (auto &tile : blocks.Items()) {
		EXPECT(tileOverlayInfo != overlayInfo.end());
		if (tile) {
			tile->SetOverlay(tileOverlayInfo->text);
			tileOverlayInfo++;
		}
	}
}

void TileGrid::ResetOverlay() {
	overlayToggled = false;
	for (auto &tile : blocks.Items()) {
		if (tile)
			tile->SetOverlay("");
	}
}

Tile *TileGrid::GetTileRel(apos pos) const {
    if (pos < apos(visibleTilesSide,visibleTilesSide,visibleTilesHeight)) {
		return blocks.At(pos).get();
    }
    return nullptr;
}

Tile *TileGrid::GetTileAbs(apos pos) const {
    return GetTileRel(pos - firstTile);
}

int TileGrid::GetTileSize() const { return tileSize; }
Object *TileGrid::GetObjectUnderCursor() const { return underCursorObject; }
//const int TileGrid::GetPaddingX() const { return padding.x; }
//const int TileGrid::GetPaddingY() const { return padding.y; }

std::unordered_map< uint, uptr<Object> > &TileGrid::GetObjects() { return objects; }

void TileGrid::updatePos(uf::vec3u pos, uf::vec3u newpos) {
	if(!blocks.At(pos)) {
		return;
	}
	blocks.At(pos)->relPos = newpos;
}
