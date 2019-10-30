from Engine.Geometry import Direction, DirectionSet, DirectionSetFractional
from Objects.Turf import Turf

class Floor(Turf):
	defName = "Floor"
	defSprite = "floor"

	def __init__(self):
		super().__init__()
		self.layer = 15
		self.solidity = DirectionSet([Direction.BOTTOM])
		self.opacity = DirectionSetFractional([(Direction.BOTTOM, 0)])
		self.airtightness = DirectionSetFractional([(Direction.BOTTOM, 0)])
