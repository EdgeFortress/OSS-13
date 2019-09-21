from Objects.Items.Clothing import Clothing, MobSlot

class Uniform(Clothing):
	def __init__(self):
		super().__init__()
		self.slot = MobSlot.UNIFORM

class Grey(Uniform):
	defName = "Grey uniform"
	defSprite = "grey"

class Captain(Uniform):
	defName = "Captain uniform"
	defSprite = "captain"

class Atmospherics(Uniform):
	defName = "Atmospherics uniform"
	defSprite = "atmospherics"

class Security(Uniform):
	defName = "Security uniform"
	defSprite = "security"

class Medic(Uniform):
	defName = "Medic uniform"
	defSprite = "medic"

class Clown(Uniform):
	defName = "Clown uniform"
	defSprite = "clown"
