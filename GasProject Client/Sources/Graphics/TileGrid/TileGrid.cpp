#include "TileGrid.hpp"

#include <map>
#include <SFML/Graphics.hpp>

#include "Shared/Global.hpp"
#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"
#include "Graphics/UI/UI.hpp"
#include "Object.hpp"
#include "Tile.hpp"
#include "Block.hpp"

TileGrid::TileGrid() :
    tileSize(0), blockSize(Global::BLOCK_SIZE), 
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
    numOfVisibleBlocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    numOfVisibleBlocks = numOfVisibleBlocks / Global::BLOCK_SIZE + (numOfVisibleBlocks % Global::BLOCK_SIZE ? 1 : 0);

    // Allocate memory for blocks
    blocks.resize(numOfVisibleBlocks);
    for (auto &vect : blocks) vect.resize(numOfVisibleBlocks);

	canBeActive = true;

    layersBuffer.resize(101);
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
            Tile *tile = GetTileAbs(cameraPos + tilePos);
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
            uf::vec2i pixel = (tile->GetRelPos() - cameraRelPos + uf::vec2i(Global::FOV / 2) - shift) * tileSize;
            object->Draw(&buffer, pixel + padding);
            if (cursorPosition >= pixel && cursorPosition < pixel + uf::vec2i(tileSize)) {
                if (!object->PixelTransparent(cursorPosition - pixel))
                    underCursorObject = object;
            }
	    }
        layerObjects.clear(); // clear buffer
    }

	buffer.display();
}

void TileGrid::SetSize(const uf::vec2i &size) {
    tileSize = int(std::min(size.x, size.y)) / Global::FOV;
    numOfTiles = { 15, 15 };
    CC::log << numOfTiles.x << numOfTiles.y << std::endl;
    padding.x = 0;
    padding.y = (int(size.y) - tileSize * numOfTiles.y) / 2;

    for (auto &object : objects) object.second->Resize(tileSize);
    for (auto &vect : blocks)
        for (auto &block : vect) {
            if (block) block->Resize(tileSize);
        }

    Widget::SetSize(uf::vec2i(tileSize) * 15);
	Widget::SetPosition(padding);
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
                moveCommand.y = -1;
				return true;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                moveCommand.y = 1;
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
    default:
        break;
    }
	return false;
}

void TileGrid::Update(sf::Time timeElapsed) {
    std::unique_lock<std::mutex> lock(mutex);

    if (actionSendPause != sf::Time::Zero) {
        actionSendPause -= timeElapsed;
        if (actionSendPause < sf::Time::Zero) actionSendPause = sf::Time::Zero;
    }

    if (actionSendPause == sf::Time::Zero) {
		if (stun == sf::Time::Zero && moveCommand) {
			Connection::commandQueue.Push(new MoveClientCommand(uf::VectToDirection(moveCommand)));

			if (controllable) {
				Tile *lastTile = controllable->GetTile();
				if (!lastTile) {
					CC::log << "Controllable isn't placed" << std::endl;
					return;
				}

				uf::vec2i moveIntent = controllable->GetMoveIntent();
				if (moveCommand.x) moveIntent.x = moveCommand.x;
				if (moveCommand.y) moveIntent.y = moveCommand.y;

				Tile *newTileX = GetTileRel({lastTile->GetRelPos().x + moveIntent.x, lastTile->GetRelPos().y});
				Tile *newTileY = GetTileRel({lastTile->GetRelPos().x, lastTile->GetRelPos().y + moveIntent.y});
				Tile *newTileDiag = GetTileRel(lastTile->GetRelPos() + moveIntent);

				if (controllable->IsDense()) {
					if (!newTileDiag || newTileDiag->IsBlocked()) moveIntent = controllable->GetMoveIntent();
					if (!newTileX || newTileX->IsBlocked()) moveIntent.x = 0;
					if (!newTileY || newTileY->IsBlocked()) moveIntent.y = 0;
				}

				controllable->SetMoveIntent(moveIntent);
				controllable->SetDirection(uf::VectToDirection(moveIntent));
			}
			else
				CC::log << "Controllable not determine" << std::endl;
		}
		moveCommand = sf::Vector2i();

        if (stun == sf::Time::Zero && objectClicked && underCursorObject)
			Connection::commandQueue.Push(new ClickObjectClientCommand(underCursorObject->GetID()));

		if (stun == sf::Time::Zero && dropButtonPressed)
			Connection::commandQueue.Push(new DropClientCommand());
		
		if (stun == sf::Time::Zero && buildButtonPressed)
			Connection::commandQueue.Push(new BuildClientCommand());

		if (ghostButtonPressed)
			Connection::commandQueue.Push(new GhostClientCommand());

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

    for (auto &vect : blocks)
        for (auto &block : vect) {
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
		throw std::exception("Unexpected!");
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

    CC::log << "Error: object with id" << id << "is not exist (TileGrid::RemoveObject)" << std::endl;
}

void TileGrid::RelocateObject(uint id, uf::vec2i toVec, int toObjectNum) {
    Tile *tile = GetTileAbs(toVec);
    if (!tile) {
        CC::log << "Wrong tile absolute coords (" << toVec.x << toVec.y << ")" << std::endl;
        return;
    }

    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        tile->AddObject(obj, toObjectNum);
        obj->ResetShiftingState();
        return;
    }

    CC::log << "Wrong object ID:" << id << std::endl;
}

void TileGrid::SetMoveIntentObject(uint id, uf::Direction direction) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        uf::vec2i dir = uf::DirectionToVect(direction);
        obj->SetMoveIntent(dir);
        return;
    }
    CC::log << "Try to set MoveIntent to unknown object (id: " << id << "(TileGrid::SetMoveIntentObject)" << std::endl;
}

void TileGrid::MoveObject(uint id, uf::Direction direction) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        Object *obj = iter->second.get();
        uf::vec2i dir = uf::DirectionToVect(direction);

        Tile *lastTile = obj->GetTile();
        if (!lastTile) {
            CC::log << "Move of unplaced object" << std::endl;
            return;
        }
        Tile *tile = GetTileRel(lastTile->GetRelPos() + dir);
        if (!tile) {
            CC::log << "Move to unknown tile" << std::endl;
            return;
        }

        //obj->SetShifting(direction, speed);
        tile->AddObject(obj, 0);
        obj->ReverseShifting(direction);
        if (obj == controllable) shift = controllable->GetShift();
        
        return;
    }
	CC::log << "Move of unknown object (id: " << id << "(TileGrid::MoveObject)" << std::endl;
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

void TileGrid::ShiftBlocks(uf::vec2i newFirst) {
    const uf::vec2i delta = newFirst - firstBlock;
    
    std::vector< std::vector< sptr<Block> > > newBlocks(numOfVisibleBlocks);
    for (auto &vect : newBlocks)
        vect.resize(numOfVisibleBlocks);

    for (int y = 0; y < numOfVisibleBlocks; y++) {
        for (int x = 0; x < numOfVisibleBlocks; x++) {
            if (x - delta.x >= 0 && x - delta.x < numOfVisibleBlocks &&
                y - delta.y >= 0 && y - delta.y < numOfVisibleBlocks) {
                newBlocks[y - delta.y][x - delta.x] = blocks[y][x];
				if (newBlocks[y - delta.y][x - delta.x]) {
                    newBlocks[y - delta.y][x - delta.x]->relPos = { x - delta.x, y - delta.y };
				}
            }
        }
    }

    blocks = std::move(newBlocks);

    firstBlock = newFirst;
    firstTile = firstBlock * blockSize;
}

void TileGrid::SetCameraPosition(uf::vec2i pos) {
    cameraPos = pos;
    cameraRelPos = pos - firstTile;
}

void TileGrid::SetBlock(uf::vec2i pos, Block *block) {
    blocks[pos.y - firstBlock.y][pos.x - firstBlock.x].reset(block);
    block->relPos = pos - firstBlock;
}

void TileGrid::SetControllable(uint id, float speed) {
    auto iter = objects.find(id);
    if (objects.find(id) != objects.end()) {
        controllable = iter->second.get();
        controllableSpeed = speed;
        return;
    }
	CC::log << "Error! New controllable wasn't founded" << std::endl;
}

Tile *TileGrid::GetTileRel(uf::vec2i rpos) const {
    if (rpos >= uf::vec2i(0) && rpos < uf::vec2i(int(blocks.size() * blockSize))) {
		auto &a = blocks[rpos.y / blockSize];
        if (!a.size())
            // Blocks are creating yet
            return nullptr;
		Block *block = a[rpos.x / blockSize].get();

        if (block) return block->GetTile(rpos.x % blockSize, rpos.y % blockSize);
    }
    return nullptr;
}

Tile *TileGrid::GetTileAbs(uf::vec2i apos) const {
    apos -= firstTile;
    return GetTileRel(apos);
}

int TileGrid::GetBlockSize() const { return blockSize; }
int TileGrid::GetTileSize() const { return tileSize; }
Object *TileGrid::GetObjectUnderCursor() const { return underCursorObject; }
//const int TileGrid::GetPaddingX() const { return padding.x; }
//const int TileGrid::GetPaddingY() const { return padding.y; }