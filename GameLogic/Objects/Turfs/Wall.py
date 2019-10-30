from Engine.Geometry import Direction, DirectionSet, DirectionSetFractional
from Objects.Turf import Turf

class Wall(Turf):
	defName = "Wall"
	defSprite = "wall"

	def __init__(self):
		super().__init__()
		self.solidity = DirectionSet([Direction.CENTER])
		self.opacity = DirectionSetFractional([(Direction.CENTER, 0)])
		self.airtightness = DirectionSetFractional([(Direction.CENTER, 0)])
