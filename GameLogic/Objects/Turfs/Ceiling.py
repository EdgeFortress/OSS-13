from Objects.Turf import Turf

class Ceiling(Turf):
	defName = "Ceiling"
	defSprite = "ceiling"

	def __init__(self):
		super().__init__()
		self.drawAtTop = True
		self.layer = 10
