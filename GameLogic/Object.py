import Engine
from datetime import timedelta

class Object(Engine.Object):
	def Update(self, timeElapsed):
		pass

	def InteractedBy(self, object):
		return False
