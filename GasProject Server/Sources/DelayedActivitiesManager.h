#pragma once

#include <chrono>
#include <functional>

struct Activity {
	std::chrono::microseconds delay;
	std::function<void()> action;
};

class DelayedActivitiesManager {
public:
	void AddDelayedActivity(std::chrono::microseconds delay, std::function<void()> action);

	virtual void Update(std::chrono::microseconds timeElapsed);

private:
	std::vector<Activity> activities;
};
