from Objects.Turf import Turf

class Floor(Turf):
	defName = "Floor"
	defSprite = "floor"

	def __init__(self):
		super().__init__()
		self.isFloor = True
		self.layer = 15
