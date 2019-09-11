import contextlib
from datetime import timedelta

from Engine_Server import GGame

def spawn(delay, activity):
	GGame.AddDelayedActivity(delay, activity)
