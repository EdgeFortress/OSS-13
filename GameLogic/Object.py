from datetime import timedelta

from Engine_World import Object as EngineObject

class Object(EngineObject):
	def __init__(self):
		super().__init__()

	def Update(self, timeElapsed):
		pass

	def InteractedBy(self, object):
		return False

	def BumpedTo(self, object):
		print(self.name + " bumped to " + object.name)
		return False
