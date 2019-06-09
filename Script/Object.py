import Engine

class Object(Engine.Object):
	def __init__(self):
		Engine.Object.__init__(self)
		print("PyObject Inited")

	def Update(self):
		pass #print("Object Updated")

	def __del__(self):
		print("PyObject Deleted")
