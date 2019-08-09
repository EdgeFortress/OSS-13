import contextlib
from datetime import timedelta

from Engine import GGame

def spawn(delay, activity):
	GGame.AddDelayedActivity(delay, activity)
