#include "Timer.h"

namespace uf {

void Timer::Start(std::chrono::microseconds time, std::function<void()> &&callback) {
	timeLeft = time;
	this->callback = callback;
}

bool Timer::IsStopped() {
	return timeLeft == timeLeft.zero();
}

bool Timer::Update(std::chrono::microseconds timeElapsed) {
	if (timeLeft == timeLeft.zero())
		return true;

	if (timeLeft > timeElapsed) {
		timeLeft -= timeElapsed;
		return false;
	} else {
		timeLeft = timeLeft.zero();
		if (callback)
			callback();
		return true;
	}
}

} // namespace uf
