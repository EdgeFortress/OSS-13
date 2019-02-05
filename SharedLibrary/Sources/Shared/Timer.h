#pragma once

#include <functional>

#include <SFML/System/Time.hpp>

namespace uf {

class Timer {
public:
	// Start timer
	void Start(sf::Time time, std::function<void()> &&callback = {});
	// true if stopped
	bool IsStopped();
	// update timer, return true if stopped
	bool Update(sf::Time timeElapsed);

private:
	sf::Time timeLeft;
	std::function<void()> callback;
};

} // namespace uf
