#include <vector>

#include <SFML/Window/Event.hpp>

#include "TileGrid.hpp"
#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Network.hpp"

#include "Shared/Math.hpp"

Object::Object(const Global::Sprite key, const string name, const bool directed) {
    SetSprite(key);
    this->name = name;
	direction = uf::Direction::NONE;
	dense = false;
}

void Object::SetSprite(const Global::Sprite key) {
    sprite = nullptr;
    if (int(key))
        for (auto &sprite : CC::Get()->GetWindow()->GetSprites())
            if (sprite->GetKey() == key) {
                this->sprite = sprite.get();
                return;
            }
}

void Object::SetDirection(const uf::Direction direction) {
	this->direction = direction;
}

void Object::SetSpeed(float speed) {
	shiftingSpeed = speed;
}

bool Object::checkObj(int x, int y) {
    return !(sprite->PixelTransparent(x, y));
}

void Object::Draw(sf::RenderWindow * const window, const int x, const int y) {
	int tileSize = CC::Get()->GetWindow()->GetTileGrid()->GetTileSize();
    if (sprite) sprite->Draw(window, x + int(shift.x * tileSize), y + int(shift.y * tileSize), direction);
}

void Object::Update(sf::Time timeElapsed) {
    //int tileSize = CC::Get()->GetWindow()->GetTileGrid()->GetTileSize();
    if (shiftingDirection.x) {
        shift.x += shiftingDirection.x * timeElapsed.asSeconds() * shiftingSpeed;
    } else {
        if (shift.x * uf::sgn(shift.x) > timeElapsed.asSeconds() * shiftingSpeed)
            shift.x -= uf::sgn(shift.x) * timeElapsed.asSeconds() * shiftingSpeed;
        else shift.x = 0;
    }
    if (shiftingDirection.y) {
        shift.y += shiftingDirection.y * timeElapsed.asSeconds() * shiftingSpeed;
    } else {
        if (shift.y * uf::sgn(shift.y) > timeElapsed.asSeconds() * shiftingSpeed)
            shift.y -= uf::sgn(shift.y) * timeElapsed.asSeconds() * shiftingSpeed;
        else shift.y = 0;
    }

    //if (shift.x * sgn(shift.x) >= 0.5) {
    //    CC::Get()->GetWindow()->GetTileGrid()->GetTileRel(tile->GetRelX(), tile->GetRelY());
    //}

    //sf::Vector2f lastShift = shift;
    //shift += timeElapsed.asSeconds() * shiftingSpeed * sf::Vector2f(shiftingDirection);

	//if (shift.x || shift.y)
		//CC::log << "Shift:" << std::to_string(shift.x) << std::to_string(shift.y) << endl;

    //if (sgn(shift.y) * sgn(lastShift.y) < 0)
        //ResetShifting();
}

void Object::ReverseShifting(uf::Direction direction) {
	sf::Vector2i directionVect = uf::DirectionToVect(direction);
	if (directionVect.x) shiftingDirection.x = 0;
	if (directionVect.y) shiftingDirection.y = 0;
    shift -= sf::Vector2f(directionVect);
}

Tile::Tile(Block *block, const int x, const int y) : 
    block(block), x(x), y(y),
    sprite(nullptr) 
{
};

void Tile::Draw(sf::RenderWindow * const window, const int x, const int y) const {
    if (sprite) sprite->Draw(window, x, y, uf::Direction::NONE);

    //for (auto &obj : content)
    //    obj->Draw(window, x, y);
}

int Tile::GetRelX() const { return block->GetRelX() * block->GetTileGrid()->GetBlockSize() + x; }
int Tile::GetRelY() const { return block->GetRelY() * block->GetTileGrid()->GetBlockSize() + y; }

Object *Tile::GetObjectByCoord(const unsigned x, const unsigned y) const {
    for (auto obj = content.rbegin(); obj != content.rend(); obj++) {
        if ((*obj)->checkObj(x, y))
            return *obj;
    }
    return nullptr;
}

Block::Block(TileGrid *tileGrid) : 
    tileGrid(tileGrid), relX(relX), relY(relY),
    tiles(tileGrid->GetBlockSize())
{
    int y = 0;
    for (auto &vect : tiles) {
        vect.resize(tileGrid->GetBlockSize());
        int x = 0;
        for (auto &tile : vect) {
            tile.reset(new Tile(this, x, y));
            x++;
        }
        y++;
    }
}

Tile* Block::GetTile(int x, int y) const {
    if (x >= 0 && x < tileGrid->GetBlockSize() && y >= 0 && y < tileGrid->GetBlockSize())
        return tiles[y][x].get();
    return nullptr;
}

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
        //for (auto &block : vect)
        //    block.reset(new Block(this));
    }
}

void TileGrid::Draw(sf::RenderWindow * const window) {
    mutex.lock();
    std::multimap<int, Object *> objects;
	int border = Global::FOV / 2 + Global::MIN_PADDING;
    for (int y = -border; y <= border; y++)
        for (int x = -border; x <= border; x++) {
            Tile *tile = GetTileRel(xRelPos + x, yRelPos + y);
            if (tile) {
                tile->Draw(window, int(xPadding + (x + Global::FOV / 2 - shift.x) * tileSize),
                                   int(yPadding + (y + Global::FOV / 2 - shift.y) * tileSize));
                for (auto &obj : tile->GetContent())
                    objects.insert(std::pair<int, Object *>(obj->GetLayer(), obj));
            }
        }
    for (auto &pair : objects)
        pair.second->Draw(window, int(xPadding + (pair.second->GetTile()->GetRelX() - xRelPos + Global::FOV / 2 - shift.x) * tileSize),
                                  int(yPadding + (pair.second->GetTile()->GetRelY() - yRelPos + Global::FOV / 2 - shift.y) * tileSize));
    mutex.unlock();
}

void TileGrid::Resize(const int windowWidth, const int windowHeight) {
    tileSize = min(windowWidth, windowHeight) / Global::FOV;
    xNumOfTiles = 15;
    yNumOfTiles = 15;
    xPadding = 0;
    yPadding = (windowHeight - tileSize * yNumOfTiles) / 2;
}

void TileGrid::HandleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                moveCommand.y = -1;
                //CC::log << moveCommand.y << endl;
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                moveCommand.y = 1;
                //CC::log << moveCommand.y << endl;
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                moveCommand.x = 1;
                //CC::log << moveCommand.x << endl;
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                moveCommand.x = -1;
                //CC::log << moveCommand.x << endl;
                break;
            default:
                break;
        }
		
    }
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
					CC::log << "Controllable isn't placed" << endl;
					return;
				}

				Tile *newTileX = GetTileRel(lastTile->GetRelX() + moveCommand.x, lastTile->GetRelY() + controllable->GetShiftingDirection().y);
				Tile *newTileY = GetTileRel(lastTile->GetRelX() + controllable->GetShiftingDirection().x, lastTile->GetRelY() + moveCommand.y);
				Tile *newTileDiag = GetTileRel(lastTile->GetRelX() + moveCommand.x, lastTile->GetRelY() + moveCommand.y);

				if (!newTileX || newTileX->IsBlocked()) moveCommand.x = 0;
				if (!newTileY || newTileY->IsBlocked()) moveCommand.y = 0;
				if (!newTileDiag || newTileDiag->IsBlocked()) moveCommand = sf::Vector2i();

				controllable->SetShifting(uf::VectToDirection(moveCommand), controllableSpeed);
			}
			else
				CC::log << "Controllable not determine" << endl;

			moveSendPause = MOVE_TIMEOUT;
			moveCommand = sf::Vector2i();
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
    for (auto iter = objects.begin(); iter != objects.end(); iter++)
        if ((*iter)->GetID() == id) {
            Object *obj = iter->get();
            if (obj->GetTile()) obj->GetTile()->RemoveObject(obj);
            objects.erase(iter);
            return;
        }
    CC::log << "Error: object with id" << id << "is not exist" << endl;
}

void TileGrid::RelocateObject(uint id, int toX, int toY, int toObjectNum) {
    Tile *tile = GetTileAbs(toX, toY);
    if (!tile) {
        CC::log << "Wrong tile absolute coords (" << toX << toY << ")" << endl;
        return;
    }
    //CC::log << "ID" << id << endl;
    for (auto &obj : objects)
        if (obj->GetID() == id) {
            tile->AddObject(obj.get(), toObjectNum);
            obj->ResetShifting();
            return;
        }
    CC::log << "Wrong object ID:" << id << endl;
}

void TileGrid::MoveObject(uint id, uf::Direction direction, float speed) {
    for (auto &obj : objects)
        if (obj->GetID() == id) {
            sf::Vector2i dir = uf::DirectionToVect(direction);
            Tile *lastTile = obj->GetTile();
            if (!lastTile) {
                CC::log << "Move of unplaced object" << endl;
                return;
            }
            Tile *tile = GetTileRel(lastTile->GetRelX() + dir.x, lastTile->GetRelY() + dir.y);
            if (!tile) {
                CC::log << "Move to unknown tile" << endl;
                return;
            }

			obj->SetSpeed(speed);

            //obj->SetShifting(direction, speed);
            tile->AddObject(obj.get(), 0);
			obj->ReverseShifting(direction);
			if (obj.get() == controllable) shift = controllable->GetShift();

            return;
        }
	CC::log << "Move of unknown object (id: " << id << "(TileGrid::MoveObject)" << endl;
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
    
    vector< vector<sptr<Block>> > newBlocks(numOfVisibleBlocks);
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
    //if (!neededBlocks) UnlockDrawing();
}

void TileGrid::SetControllable(uint id, float speed) {
    for (auto &obj : objects)
        if (obj->GetID() == id) {
            controllable = obj.get();
            controllableSpeed = speed;
            return;
        }
    CC::log << "Error! New controllable not founded" << endl;
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
		//CC::log << "GetTileRel" << x << y << endl;
		auto &a = blocks[y / blockSize];
		if (!a.size())
			CC::log << "ALERT" << endl;
		auto &b = a[x / blockSize];
		auto block = b.get();
        //Block *block = blocks[y / blockSize][x / blockSize].get();
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