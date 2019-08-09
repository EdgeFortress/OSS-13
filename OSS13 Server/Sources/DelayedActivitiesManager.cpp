#include "DelayedActivitiesManager.h"

void DelayedActivitiesManager::AddDelayedActivity(std::chrono::microseconds delay, std::function<void()> action) {
	activities.push_back(Activity{delay, action});
}

void DelayedActivitiesManager::Update(std::chrono::microseconds timeElapsed) {
	for (auto activity = activities.begin(); activity != activities.end(); ) {
		activity->delay -= timeElapsed;
		if (activity->delay < activity->delay.zero()) {
			activity->action();
			activity = activities.erase(activity);
		} else {
			activity++;
		}
	}
}
