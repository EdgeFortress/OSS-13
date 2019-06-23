from Creature.Creature import Creature

class Ghost(Creature):
	def __init__(self):
		Creature.__init__(self)
		self.layer = 80
		self.name = "Python Ghost"
		self.sprite = "ghost"
		self.density = False
		self.seeInvisibleAbility = 1
		self.invisibility = 1
