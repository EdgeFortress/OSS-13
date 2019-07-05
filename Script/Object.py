import Engine
from datetime import timedelta

class Object(Engine.Object):
	def __init__(self):
		super().__init__()
		print("PyObject Inited")

	def Update(self, timeElapsed):
		pass

	def InteractedBy(self, object):
		return False

	def __del__(self):
		print("PyObject Deleted")
