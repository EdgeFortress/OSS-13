#pragma once

#include <functional>
#include <chrono>

namespace uf {

class Timer {
public:
	// Start timer
	void Start(std::chrono::microseconds, std::function<void()> &&callback = {});
	// true if stopped
	bool IsStopped();
	// update timer, return true if stopped
	bool Update(std::chrono::microseconds timeElapsed);

private:
	std::chrono::microseconds timeLeft;
	std::function<void()> callback;
};

} // namespace uf
