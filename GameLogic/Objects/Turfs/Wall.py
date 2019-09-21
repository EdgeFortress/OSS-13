from Engine.Geometry import Direction
from Objects.Turf import Turf

class Wall(Turf):
	canBeSpawned = True
	defName = "Wall"
	defSprite = "wall"

	def __init__(self):
		super().__init__()
		self.isWall = True
		self.solidity.Add([Direction.CENTER])
