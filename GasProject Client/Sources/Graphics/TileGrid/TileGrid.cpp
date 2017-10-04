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
    controllable(nullptr), controllableSpeed(0),
    clickedObject(nullptr), buildButtonPressed(false), ghostButtonPressed(false)
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

    clickedObject = nullptr;
	ghostButtonPressed = false;
	buildButtonPressed = false;
}

void TileGrid::draw() const {
    mutex.lock();
	buffer.clear();
    std::multimap<int, Object *> objects;
	const int border = Global::FOV / 2 + Global::MIN_PADDING;
    // tiles positions relative to camera
    uf::vec2i tilePos;
    for (tilePos.y = -border; tilePos.y <= border; tilePos.y++)
        for (tilePos.x = -border; tilePos.x <= border; tilePos.x++) {
            Tile *tile = GetTileAbs(cameraPos + tilePos);
            if (tile) {
                tile->Draw(&buffer, padding + (tilePos + uf::vec2i(Global::FOV / 2) - shift) * tileSize);
                for (auto &obj : tile->GetContent())
                    objects.insert(std::pair<int, Object *>(obj->GetLayer(), obj));
            }
        }
	for (auto &pair : objects) {
        Tile *tile = pair.second->GetTile();
        pair.second->Draw(&buffer, padding + (tile->GetRelPos() - cameraRelPos + uf::vec2i(Global::FOV / 2) - shift) * tileSize);
	}
	buffer.display();
    mutex.unlock();
}

void TileGrid::SetSize(const uf::vec2i &size) {
    tileSize = int(std::min(size.x, size.y)) / Global::FOV;
    numOfTiles = { 15, 15 };
    CC::log << numOfTiles.x << numOfTiles.y << std::endl;
    padding.x = 0;
    padding.y = (int(size.y) - tileSize * numOfTiles.y) / 2;
	CC::Get()->RM.SpritesResize(tileSize);
    Widget::SetSize(uf::vec2i(tileSize) * 15);
	Widget::SetPosition(padding);
}

bool TileGrid::HandleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
        CC::log << "CHECK" << std::endl;
		const uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()) {
            clickedObject = GetObjectByPixel({event.mouseButton.x, event.mouseButton.y});
            if (clickedObject) CC::log << clickedObject->GetName() << std::endl;
			return true;
        }
		break;
	}
	case sf::Event::KeyPressed: {
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
	mutex.lock();
    if (actionSendPause != sf::Time::Zero) {
        actionSendPause -= timeElapsed;
        if (actionSendPause < sf::Time::Zero) actionSendPause = sf::Time::Zero;
    }

    if (actionSendPause == sf::Time::Zero) {
		if (moveCommand.x || moveCommand.y) {
			Connection::commandQueue.Push(new MoveClientCommand(uf::VectToDirection(moveCommand)));

			if (controllable) {
				Tile *lastTile = controllable->GetTile();
				if (!lastTile) {
					CC::log << "Controllable isn't placed" << std::endl;
					return;
				}

                Tile *newTileX = GetTileRel({lastTile->GetRelPos().x + moveCommand.x, lastTile->GetRelPos().y + controllable->GetShiftingDirection().y});
                Tile *newTileY = GetTileRel({lastTile->GetRelPos().x + controllable->GetShiftingDirection().x, lastTile->GetRelPos().y + moveCommand.y});
                Tile *newTileDiag = GetTileRel(lastTile->GetRelPos() + moveCommand);

				if (controllable->IsDense()) {
					if (!newTileX || newTileX->IsBlocked()) moveCommand.x = 0;
					if (!newTileY || newTileY->IsBlocked()) moveCommand.y = 0;
					if (!newTileDiag || newTileDiag->IsBlocked()) moveCommand = sf::Vector2i();
				}

				controllable->SetShifting(uf::VectToDirection(moveCommand), controllableSpeed);
			}
			else
				CC::log << "Controllable not determine" << std::endl;

			moveCommand = sf::Vector2i();
		}
        if (clickedObject) {
            Connection::commandQueue.Push(new ClickObjectClientCommand(clickedObject->GetID()));
            clickedObject = nullptr;
        }
		if (buildButtonPressed) {
			Connection::commandQueue.Push(new BuildClientCommand());
			buildButtonPressed = false;
		}
		if (ghostButtonPressed) {
			Connection::commandQueue.Push(new GhostClientCommand());
			ghostButtonPressed = false;
		}
        actionSendPause = ACTION_TIMEOUT;
    }

    
    for (auto iter = objects.begin(); iter != objects.end();) {
        (*iter)->Update(timeElapsed);
        if (!(*iter)->GetTile()) iter = objects.erase(iter);
        else iter++;
    }
    
	if (controllable) shift = controllable->GetShift();
	mutex.unlock();
}

void TileGrid::LockDrawing() {
    mutex.lock();
}

void TileGrid::UnlockDrawing() {
    mutex.unlock();
}

void TileGrid::AddObject(Object *object) {
    objects.push_back(uptr<Object>(object));
}

void TileGrid::RemoveObject(uint id) {
	// TODO: Crutch to be removed
	if (!objects.empty()) {
		for (auto iter = objects.begin(); iter != objects.end(); iter++)
			if ((*iter)->GetID() == id) {
				Object *obj = iter->get();
				if (obj->GetTile()) obj->GetTile()->RemoveObject(obj);
				objects.erase(iter);
				return;
			}
		CC::log << "Error: object with id" << id << "is not exist" << std::endl;
	}
}

void TileGrid::RelocateObject(uint id, uf::vec2i toVec, int toObjectNum) {
    Tile *tile = GetTileAbs(toVec);
    if (!tile) {
        CC::log << "Wrong tile absolute coords (" << toVec.x << toVec.y << ")" << std::endl;
        return;
    }

    for (auto &obj : objects)
        if (obj->GetID() == id) {
            tile->AddObject(obj.get(), toObjectNum);
            obj->ResetShifting();
            return;
        }
    CC::log << "Wrong object ID:" << id << std::endl;
}

void TileGrid::MoveObject(uint id, uf::Direction direction, float speed) {
    for (auto &obj : objects)
        if (obj->GetID() == id) {
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

			obj->SetSpeed(speed);

            //obj->SetShifting(direction, speed);
            tile->AddObject(obj.get(), 0);
			obj->ReverseShifting(direction);
			if (obj.get() == controllable) shift = controllable->GetShift();

            return;
        }
	CC::log << "Move of unknown object (id: " << id << "(TileGrid::MoveObject)" << std::endl;
}

void TileGrid::ChangeObjectDirection(uint id, uf::Direction direction) {
	for (auto &obj : objects)
		if (obj->GetID() == id) {
			obj->SetDirection(direction);
			return;
		}
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
	for (auto &obj : objects) {
		if (obj->GetID() == id) {
			controllable = obj.get();
			controllableSpeed = speed;
			return;
		}
	}
	CC::log << "Error! New controllable wasn't founded" << std::endl;
}

//wptr<Block> TileGrid::GetBlock(const int blockX, const int blockY) const {
//    int relBlockX = blockX - firstBlockX;
//    int relBlockY = blockY - firstBlockY;
//
//    if (relBlockX < 0 || relBlockX >= numOfVisibleBlocks ||
//        relBlockY < 0 || relBlockY >= numOfVisibleBlocks)
//        return wptr<Block>();
//
//    return wptr<Block>(blocks[relBlockY][relBlockX]);
//}

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

Object *TileGrid::GetObjectByPixel(uf::vec2i pixel) const {
    pixel -= padding;
    const uf::vec2i tile = pixel / tileSize + cameraRelPos - numOfTiles / 2;
    Tile *locTile = GetTileRel(tile);
    if (!locTile) return nullptr;

    const uf::vec2i tilePixel = pixel % tileSize;
    Object *object = locTile->GetObjectByPixel(tilePixel);

    return object;
}

const int TileGrid::GetBlockSize() const { return blockSize; }
const int TileGrid::GetTileSize() const { return tileSize; }
//const int TileGrid::GetPaddingX() const { return padding.x; }
//const int TileGrid::GetPaddingY() const { return padding.y; }