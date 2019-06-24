import Engine
from datetime import timedelta

class Object(Engine.Object):
	def __init__(self):
		Engine.Object.__init__(self)
		print("PyObject Inited")

	def Update(self, timedelta):
		pass

	def __del__(self):
		print("PyObject Deleted")
