import contextlib
from datetime import timedelta
from typing import Callable

from Engine_Server import eGGame

def spawn(delay: timedelta, activity: Callable[[], None]) -> None:
	"""
	Schedule delayed activity

	Parameteres
	-----------
	delay: timedelta
		time do activity will be called

	activity: Callable[[], None]
		activity which should be called

	"""

	eGGame.AddDelayedActivity(delay, activity)
