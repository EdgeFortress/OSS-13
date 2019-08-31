#include "TileGrid.hpp"

#include <map>
#include <SFML/Graphics.hpp>

#include <Shared/Array.hpp>
#include <Shared/Global.hpp>
#include <Shared/IFaces/IConfig.h>
#include <Shared/Network/Protocol/ClientToServer/Commands.h>

#include <Client.hpp>
#include <ResourceManager.hpp>
#include <Network.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/UI/UI.hpp>

#include "Object.hpp"
#include "Tile.hpp"

using namespace network::protocol;

TileGrid::TileGrid() :
    tileSize(0), overlayToggled(false),
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
    visibleTilesSide = Global::FOV + 2 * Global::MIN_PADDING;
    visibleTilesHeight = Global::Z_FOV | 1;

    // Allocate memory for blocks
    blocks.resize(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

	canBeActive = true;

    layersBuffer.resize(101);

	movementPredictionDisabled = CC::Get()->RM.Config()->GetBool("Debug.MovementPredictionDisabled");
}

void TileGrid::draw() const {
    std::unique_lock<std::mutex> lock(mutex);

    underCursorObject = nullptr;
	buffer.clear();
	const int border = Global::FOV / 2 + Global::MIN_PADDING;

    // Firstly, fill layers buffer by objects
    uf::vec2i tilePos; // tiles positions relative to camera
    for (tilePos.y = -border; tilePos.y <= border; tilePos.y++)
        for (tilePos.x = -border; tilePos.x <= border; tilePos.x++) {
            Tile *tile = GetTileAbs(cameraPos + rpos(tilePos, cameraZ));
            if (tile) {
                tile->Draw(&buffer, padding + (tilePos + uf::vec2i(Global::FOV / 2) - shift) * tileSize);
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
            if ((tile->GetRelPos() - cameraRelPos).z != cameraZ) {
				continue;
			}
            uf::vec2i pixel = (uf::vec2i(Global::FOV / 2) + rpos(tile->GetRelPos() - cameraRelPos).xy() - shift) * tileSize;
            object->Draw(&buffer, pixel + padding);
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
					tile->DrawOverlay(&buffer, padding + (tilePos + uf::vec2i(Global::FOV / 2) - shift) * tileSize);
				}
			}
	}

	buffer.display();
}

void TileGrid::SetSize(const uf::vec2i &size) {
    tileSize = int(std::min(size.x, size.y)) / Global::FOV;
    numOfTiles = { 15, 15 };
    padding.x = 0;
    padding.y = (int(size.y) - tileSize * numOfTiles.y) / 2;

    for (auto &object : objects) object.second->Resize(tileSize);
	for (auto &block : blocks) {
		if (block) block->Resize(tileSize);
	}

	CustomWidget::SetSize(uf::vec2i(tileSize) * 15);
	CustomWidget::SetPosition(padding);
}

bool TileGrid::HandleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
        if (underCursorObject) {
            objectClicked = true;
            return true;
        }
        return false;
	}
    case sf::Event::MouseMoved: {
        cursorPosition = { event.mouseMove.x, event.mouseMove.y };
        cursorPosition -= GetAbsPosition() + padding;
        if (cursorPosition <= GetSize()) return true;
        else {
            cursorPosition = { -1, -1 };
            return false;
        }
    }
    case sf::Event::MouseLeft: {
        cursorPosition = { -1, -1 };
        return true;
    }
	case sf::Event::KeyPressed: {
		if (event.key.control) {
			switch (event.key.code) {
				case sf::Keyboard::D:
					dropButtonPressed = true;
					return true;
			}
		}
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                if (event.key.shift) {
					moveZCommand = 1;
				} else {
					moveCommand.y = -1;
				}
				return true;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                if (event.key.shift) {
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
	}
    case sf::Event::MouseWheelScrolled: {
		int newCameraZ = cameraZ + int(event.mouseWheelScroll.delta) % 2;
		if (GetTileRel(cameraRelPos+rpos(0,0,newCameraZ))) {
			cameraZ = newCameraZ;
		}
		return true;
	}
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

		if (lastTile->IsBlocked({ moveDirection })) { // exit from current tile
			moveIntent = controllable->GetMoveIntent();
		} else {
			if (!newTileDiag || newTileDiag->IsBlocked({ uf::InvertDirection(moveDirection), uf::Direction::CENTER })) {
				return;
			} else {
				if (!newTileX || newTileX != lastTile && newTileX->IsBlocked({ uf::InvertDirection(xDirection), yDirection, uf::Direction::CENTER })) return;
				if (!newTileY || newTileY != lastTile && newTileY->IsBlocked({ uf::InvertDirection(yDirection), xDirection, uf::Direction::CENTER })) return;
			}
		}
	}

	controllable->SetMoveIntent(moveIntent, false);
}

void TileGrid::Update(sf::Time timeElapsed) {
    std::unique_lock<std::mutex> lock(mutex);

    if (actionSendPause != sf::Time::Zero) {
        actionSendPause -= timeElapsed;
        if (actionSendPause < sf::Time::Zero) actionSendPause = sf::Time::Zero;
    }

    if (actionSendPause == sf::Time::Zero) {
		if (stun == sf::Time::Zero && moveCommand) {
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

	for (auto &block : blocks) {
		if (block) block->Update(timeElapsed);
	}
    
	if (controllable) shift = controllable->GetShift();


	if (stun > timeElapsed)
		stun -= timeElapsed;
	else
		stun = sf::Time::Zero;
}

void TileGrid::LockDrawing() {
    mutex.lock();
}

void TileGrid::UnlockDrawing() {
    mutex.unlock();
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

void TileGrid::RelocateObject(uint id, apos toVec, int toObjectNum) {
    Tile *tile = GetTileAbs(toVec);
    if (!tile) {
        LOGE << "Wrong tile absolute coords (" << toVec << ")";
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
    const rpos delta = newFirst - firstTile;
    
    std::vector< sptr<Tile> > newBlocks(visibleTilesSide*visibleTilesSide*visibleTilesHeight);

    for (int x = 0; x < visibleTilesSide; x++) {
        for (int y = 0; y < visibleTilesSide; y++) {
			for (int z = 0; z < visibleTilesHeight; z++) {
				if (x - delta.x >= 0 && x - delta.x < visibleTilesSide &&
					y - delta.y >= 0 && y - delta.y < visibleTilesSide &&
					z - delta.z >= 0 && z - delta.z < visibleTilesHeight) {
					const uint i = flat_index(apos(x, y, z) - delta);
					newBlocks[i] = blocks[flat_index(apos(x,y,z))];
					if (newBlocks[i]) {
						newBlocks[i]->relPos = apos(x, y, z) - delta;
					}
				}
            }
        }
    }

    blocks = std::move(newBlocks);

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
    blocks[flat_index(pos - firstTile)] = tile;
    tile->relPos = pos - firstTile;
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

void TileGrid::UpdateOverlay(std::vector<network::protocol::OverlayInfo> &overlayInfo) {
	overlayToggled = true;
	auto tileOverlayInfo = overlayInfo.begin();
	for (auto &tile : blocks) {
		EXPECT(tileOverlayInfo != overlayInfo.end());
		if (tile) {
			tile->SetOverlay(tileOverlayInfo->text);
			tileOverlayInfo++;
		}
	}
}

void TileGrid::ResetOverlay() {
	overlayToggled = false;
	for (auto &tile : blocks) {
		if (tile)
			tile->SetOverlay("");
	}
}

Tile *TileGrid::GetTileRel(apos pos) const {
    if (pos < apos(visibleTilesSide,visibleTilesSide,visibleTilesHeight)) {
		return blocks[flat_index(pos)].get();
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

uint TileGrid::flat_index(const apos c) const {
	return uf::flat_index(c, visibleTilesSide, visibleTilesSide);
}
