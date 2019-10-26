from Engine.Geometry import Direction, DirectionSet
from Objects.Turf import Turf

class Wall(Turf):
	defName = "Wall"
	defSprite = "wall"

	def __init__(self):
		super().__init__()
		self.isWall = True
		self.solidity = DirectionSet([Direction.CENTER])
