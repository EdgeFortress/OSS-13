#include "Timer.h"

namespace uf {

void Timer::Start(sf::Time time, std::function<void()> &&callback) {
	timeLeft = time;
	this->callback = callback;
}

bool Timer::IsStopped() {
	return timeLeft == sf::Time::Zero;
}

bool Timer::Update(sf::Time timeElapsed) {
	if (timeLeft == sf::Time::Zero)
		return true;

	if (timeLeft > timeElapsed) {
		timeLeft -= timeElapsed;
		return false;
	} else {
		timeLeft = sf::Time::Zero;
		if (callback)
			callback();
		return true;
	}
}

} // namespace uf
