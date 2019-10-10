from Engine.Geometry import Direction, DirectionSet
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

	def Close(self):
		self.isOpened = False
		self.solidity = DirectionSet([Direction.CENTER])
		self.sprite = self.defSprite
