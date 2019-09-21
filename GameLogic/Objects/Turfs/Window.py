from Engine.Geometry import Direction
from Engine.World import Object

from Objects.Turf import Turf

class Window(Turf):
	defName = "Window"
	defSprite = "window"

	def __init__(self):
		super().__init__()
		self.layer = 80
		self.solidity.Add([Direction.SOUTH])
