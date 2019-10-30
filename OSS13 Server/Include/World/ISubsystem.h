#pragma once

#include <chrono>

#include <Shared/IFaces/IFace.h>

struct ISubsystem : IFace {
	virtual void Update(std::chrono::microseconds timeElapsed) = 0;
};
