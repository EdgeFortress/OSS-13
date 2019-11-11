from Engine.World import Object

class Item(Object):
	def __init__(self):
		super().__init__()
		self.layer = 50
	
	def InteractWith(self, object):
		return False

	def ClickTile(self, tile):
		return False
