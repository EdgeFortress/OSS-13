from Engine.Geometry import Direction, DirectionSet, DirectionSetFractional
from Objects.Turf import Turf

class Ceiling(Turf):
	defName = "Ceiling"
	defSprite = "ceiling"

	def __init__(self):
		super().__init__()
		self.drawAtTop = True
		self.layer = 10
		self.solidity = DirectionSet([Direction.TOP])
		self.opacity = DirectionSetFractional([(Direction.TOP, 0)])
		self.airtightness = DirectionSetFractional([(Direction.TOP, 0)])
