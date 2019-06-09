from Object import Object

class Item(Object):
	def __init__(self):
		Object.__init__(self)
		self.layer = 50
		self.density = False
