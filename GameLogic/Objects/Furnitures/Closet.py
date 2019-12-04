from Engine.Geometry import Direction, DirectionSet
from Objects.Item import Item
from Objects.Furniture import Furniture

class Closet(Furniture):
	defName = "Closet"
	defSprite = "closet"

	def __init__(self):
		super().__init__()
		self.solidity = DirectionSet([Direction.CENTER])
		self.isOpened = False

	def InteractedBy(self, object):
		if not self.IsCloseTo(object):
			return False

		if self.isOpened and isinstance(object, Item):
			object.tile = self.tile
			return True

		self.Activate()
		return True

	def Activate(self):
		if self.isOpened:
			self.Close()
		else:
			self.Open()

	def Open(self):
		self.isOpened = True
		self.solidity = DirectionSet()
		self.sprite = "closet_opened"

		for o in self.content:
			o.tile = self.tile

	def Close(self):
		self.isOpened = False
		self.solidity = DirectionSet([Direction.CENTER])
		self.sprite = self.defSprite

		for o in self.tile.content:
			if o is self:
				continue
			if not isinstance(o, Item):
				continue
			self.AddObject(o)
