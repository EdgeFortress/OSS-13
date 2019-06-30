from Creature.Creature import Creature
from Engine import CreateObject, Vec2i

class Ghost(Creature):
	def __init__(self):
		Creature.__init__(self)
		self.layer = 80
		self.name = "Python Ghost"
		self.sprite = "ghost"
		self.density = False
		self.seeInvisibleAbility = 1
		self.invisibility = 1

def Ghostize(player):
	pos = player.GetControl().GetOwner().position
	ghost = CreateObject("Creature.Ghost", pos)
	print(player.ckey + " wants to be a ghost, wow!")
