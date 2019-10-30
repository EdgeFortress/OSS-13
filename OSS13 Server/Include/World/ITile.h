#pragma once

#include <chrono>
#include <list>

#include <Shared/IFaces/IFace.h>
#include <Shared/Geometry/DirectionSet.h>

class Object;
class Map;

struct ITile : public IFace {
	virtual void Update(std::chrono::microseconds timeElapsed) = 0;

	virtual bool MoveTo(Object *) = 0;
	virtual void PlaceTo(Object *) = 0;
	virtual bool RemoveObject(Object *obj) = 0;

	virtual const std::list<Object *> &Content() const = 0;

	virtual uf::vec3i GetPos() const = 0;
	virtual ITile *StepTo(uf::Direction direction) const = 0;
	virtual Map *GetMap() const = 0;

	virtual bool IsDense(uf::DirectionSet directions) const = 0;
	virtual Object *GetDenseObject(uf::DirectionSet directions) const = 0;
};
