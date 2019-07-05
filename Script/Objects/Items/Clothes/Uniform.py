from Objects.Items.Clothing import Clothing, MobSlot

class Uniform(Clothing):
	def __init__(self):
		super().__init__()
		self.slot = MobSlot.UNIFORM
		self.sprite = "grey"
		self.name = "uniform"
