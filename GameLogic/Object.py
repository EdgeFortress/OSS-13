from datetime import timedelta

import Engine

class Object(Engine.Object):
	def __init__(self):
		super().__init__()

	def Update(self, timeElapsed):
		pass

	def InteractedBy(self, object):
		return False
