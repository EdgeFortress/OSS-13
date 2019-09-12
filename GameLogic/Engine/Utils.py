import contextlib
from datetime import timedelta
from typing import Callable

from Engine_Server import eGGame

def spawn(delay: timedelta, activity: Callable[[], None]) -> None:
	eGGame.AddDelayedActivity(delay, activity)
