from Engine.Geometry import Direction, DirectionSet
from Engine.World import Object

from Objects.Turf import Turf

class Window(Turf):
	defName = "Window"
	defSprite = "window"
	defRotatable = True

	def __init__(self):
		super().__init__()
		self.layer = 80
		self.solidity = DirectionSet([Direction.SOUTH])
