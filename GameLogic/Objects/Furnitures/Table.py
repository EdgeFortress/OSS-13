from Engine.Geometry import Direction, DirectionSet
from Objects.Item import Item
from Objects.Furniture import Furniture

class Table(Furniture):
	defName = "Table"
	defSprite = "table"

	def __init__(self):
		super().__init__()
		self.solidity = DirectionSet([Direction.CENTER])

	def InteractedBy(self, object):
		if not self.IsCloseTo(object):
			return False
		if isinstance(object, Item):
			object.tile = self.tile
			return True
		return True

	def Activate(self):
		pass

