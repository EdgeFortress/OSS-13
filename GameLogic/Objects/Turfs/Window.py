from Engine_Geometry import Direction
from Engine.World import Object

from Objects.Turf import Turf

class Window(Turf):
	def __init__(self):
		super().__init__()
		self.name = "Window"
		self.sprite = "window"
		self.layer = 80
		self.solidity.Add([Direction.SOUTH])
