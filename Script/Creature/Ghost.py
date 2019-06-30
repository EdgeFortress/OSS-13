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

	@property
	def hostControl(self):
		return self._hostControl

	@hostControl.setter
	def hostControl(self, value):
		self._hostControl = value


def Ghostize(player):
	mob = player.control.GetOwner()

	if not isinstance(mob, Ghost):
		pos = player.control.GetOwner().position
		ghost = CreateObject("Creature.Ghost", pos)
		ghost.hostControl = player.control
		player.control = ghost.GetComponent("Control")
	else:
		ghost = mob
		player.control = ghost.hostControl
		ghost.Delete()
