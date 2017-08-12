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
    blockSize(Global::BLOCK_SIZE),
    firstBlockX(0), firstBlockY(0)
{
    //
    // Count num of visible blocks
    //
    // num * size - (2 * pad + fov) >= size
    // num >= (size + 2 * pad + fov) / size
    // We need minimal num, so add 1 if not divided
    numOfVisibleBlocks = Global::BLOCK_SIZE + Global::FOV + 2 * Global::MIN_PADDING;
    numOfVisibleBlocks = numOfVisibleBlocks / Global::BLOCK_SIZE + (numOfVisibleBlocks % Global::BLOCK_SIZE ? 1 : 0);

    blocks.resize(numOfVisibleBlocks);

    for (auto &vect : blocks) {
        vect.resize(numOfVisibleBlocks);
    }

	canBeActive = true;
}

void TileGrid::draw() const {
	mutex.lock();
	buffer.clear();
    std::multimap<int, Object *> objects;
	int border = Global::FOV / 2 + Global::MIN_PADDING;
    for (int y = -border; y <= border; y++)
        for (int x = -border; x <= border; x++) {
            Tile *tile = GetTileRel(xRelPos + x, yRelPos + y);
            if (tile) {
                tile->Draw(&buffer, int(xPadding + (x + Global::FOV / 2 - shift.x) * tileSize),
                                    int(yPadding + (y + Global::FOV / 2 - shift.y) * tileSize));
                for (auto &obj : tile->GetContent())
                    objects.insert(std::pair<int, Object *>(obj->GetLayer(), obj));
            }
        }
	for (auto &pair : objects) {
		pair.second->Draw(&buffer, int(xPadding + (pair.second->GetTile()->GetRelX() - xRelPos + Global::FOV / 2 - shift.x) * tileSize),
								   int(yPadding + (pair.second->GetTile()->GetRelY() - yRelPos + Global::FOV / 2 - shift.y) * tileSize));
	}
	buffer.display();
	mutex.unlock();
}

void TileGrid::SetSize(const sf::Vector2f &size) {
    tileSize = int(std::min(size.x, size.y)) / Global::FOV;
    xNumOfTiles = 15;
    yNumOfTiles = 15;
    xPadding = 0;
    yPadding = (int(size.y) - tileSize * yNumOfTiles) / 2;
	CC::Get()->GetWindow()->SetSpriteSize(tileSize);
	Widget::SetSize(uf::vec2i(tileSize) * 15);
	Widget::SetPosition(uf::vec2i(0, (size.y - GetSize().y) / 2));
}

bool TileGrid::HandleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
		uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize())
			return true;
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
		case sf::Keyboard::G:
			ghostButtonPressed = true;
			return true;
		default:
			break;
		}
	}
	}
	return false;
}

void TileGrid::Update(sf::Time timeElapsed) {
	mutex.lock();
    if (moveSendPause != sf::Time::Zero) {
        moveSendPause -= timeElapsed;
        if (moveSendPause < sf::Time::Zero) moveSendPause = sf::Time::Zero;
    }

    if (moveSendPause == sf::Time::Zero) {
		if (moveCommand.x || moveCommand.y) {
			Connection::commandQueue.Push(new MoveClientCommand(uf::VectToDirection(moveCommand)));

			if (controllable) {
				Tile *lastTile = controllable->GetTile();
				if (!lastTile) {
					CC::log << "Controllable isn't placed" << std::endl;
					return;
				}

				Tile *newTileX = GetTileRel(lastTile->GetRelX() + moveCommand.x, lastTile->GetRelY() + controllable->GetShiftingDirection().y);
				Tile *newTileY = GetTileRel(lastTile->GetRelX() + controllable->GetShiftingDirection().x, lastTile->GetRelY() + moveCommand.y);
				Tile *newTileDiag = GetTileRel(lastTile->GetRelX() + moveCommand.x, lastTile->GetRelY() + moveCommand.y);

				if (controllable->IsDense()) {
					if (!newTileX || newTileX->IsBlocked()) moveCommand.x = 0;
					if (!newTileY || newTileY->IsBlocked()) moveCommand.y = 0;
					if (!newTileDiag || newTileDiag->IsBlocked()) moveCommand = sf::Vector2i();
				}

				controllable->SetShifting(uf::VectToDirection(moveCommand), controllableSpeed);
			}
			else
				CC::log << "Controllable not determine" << std::endl;

			moveSendPause = MOVE_TIMEOUT;
			moveCommand = sf::Vector2i();
		}
		if (ghostButtonPressed) {
			Connection::commandQueue.Push(new GhostClientCommand());
			moveSendPause = MOVE_TIMEOUT;
			ghostButtonPressed = false;
		}
    }

    
    for (auto &iter = objects.begin(); iter != objects.end();) {
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

void TileGrid::RelocateObject(uint id, int toX, int toY, int toObjectNum) {
    Tile *tile = GetTileAbs(toX, toY);
    if (!tile) {
        CC::log << "Wrong tile absolute coords (" << toX << toY << ")" << std::endl;
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
            sf::Vector2i dir = uf::DirectionToVect(direction);
            Tile *lastTile = obj->GetTile();
            if (!lastTile) {
                CC::log << "Move of unplaced object" << std::endl;
                return;
            }
            Tile *tile = GetTileRel(lastTile->GetRelX() + dir.x, lastTile->GetRelY() + dir.y);
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

void TileGrid::ShiftBlocks(const int newFirstX, const int newFirstY) {
    int dx = newFirstX - firstBlockX;
    int dy = newFirstY - firstBlockY;
    
    std::vector< std::vector< sptr<Block> > > newBlocks(numOfVisibleBlocks);
    for (auto &vect : newBlocks)
        vect.resize(numOfVisibleBlocks);

    for (int y = 0; y < numOfVisibleBlocks; y++) {
        for (int x = 0; x < numOfVisibleBlocks; x++) {
            if (x - dx >= 0 && x - dx < numOfVisibleBlocks &&
                y - dy >= 0 && y - dy < numOfVisibleBlocks) {
                newBlocks[y - dy][x - dx] = blocks[y][x];
				if (newBlocks[y - dy][x - dx]) {
					newBlocks[y - dy][x - dx]->relX = x - dx;
					newBlocks[y - dy][x - dx]->relY = y - dy;
				}
            }
        }
    }

    blocks = std::move(newBlocks);

    firstBlockX = newFirstX;
    firstBlockY = newFirstY;

    firstTileX = firstBlockX * blockSize;
    firstTileY = firstBlockY * blockSize;
}

void TileGrid::SetCameraPosition(const int x, const int y) {
    xPos = x;
    yPos = y;
    xRelPos = x - firstTileX;
    yRelPos = y - firstTileY;
}

void TileGrid::SetBlock(int x, int y, Block *block) {
    blocks[y - firstBlockY][x - firstBlockX].reset(block);
    block->relX = x - firstBlockX;
    block->relY = y - firstBlockY;
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

wptr<Block> TileGrid::GetBlock(const int blockX, const int blockY) const {
    int relBlockX = blockX - firstBlockX;
    int relBlockY = blockY - firstBlockY;

    if (relBlockX < 0 || relBlockX >= numOfVisibleBlocks ||
        relBlockY < 0 || relBlockY >= numOfVisibleBlocks)
        return wptr<Block>();

    return wptr<Block>(blocks[relBlockY][relBlockX]);
}

Tile *TileGrid::GetTileRel(int x, int y) const {
    if (x >= 0 && x < blocks.size() * blockSize && y >= 0 && y < blocks.size() * blockSize) {
		auto &a = blocks[y / blockSize];
		if (!a.size())
			CC::log << "ALERT" << std::endl;
		auto &b = a[x / blockSize];
		auto block = b.get();

        if (block)
            return block->GetTile(x % blockSize, y % blockSize);
        else
            return nullptr;
    }
    return nullptr;
}

Tile *TileGrid::GetTileAbs(int x, int y) const {
    x -= firstTileX;
    y -= firstTileY;
    return GetTileRel(x, y);
}

Object *TileGrid::GetObjectByPixel(int x, int y) const {
    int xTile = (x - xPadding) / tileSize + xRelPos - xNumOfTiles / 2;
    int yTile = (y - yPadding) / tileSize + yRelPos - yNumOfTiles / 2;
    Tile *locTile = GetTileRel(xTile, yTile);
    if (locTile == nullptr) return nullptr;

    int xLoc = (x - xPadding) % tileSize;
    int yLoc = (y - yPadding) % tileSize;
    Object *locObj = locTile->GetObjectByCoord(xLoc, yLoc);

    return locObj;
}

const int TileGrid::GetBlockSize() const { return blockSize; }
const int TileGrid::GetTileSize() const { return tileSize; }
const int TileGrid::GetWidth() const { return tileSize * xNumOfTiles; }
const int TileGrid::GetHeigth() const { return tileSize * yNumOfTiles; }
const int TileGrid::GetPaddingX() const { return xPadding; }
const int TileGrid::GetPaddingY() const { return yPadding; }