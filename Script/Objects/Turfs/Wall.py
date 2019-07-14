from Objects.Turf import Turf

class Wall(Turf):
	def __init__(self):
		super().__init__()
		self.isWall = True
		self.name = "Wall"
		self.sprite = "wall"
		self.density = True
