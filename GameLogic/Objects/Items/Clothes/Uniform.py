from Objects.Items.Clothing import Clothing, MobSlot

class Uniform(Clothing):
	canBeSpawned = True
	defName = "Uniform"
	defSprite = "grey"

	def __init__(self):
		super().__init__()
		self.slot = MobSlot.UNIFORM
