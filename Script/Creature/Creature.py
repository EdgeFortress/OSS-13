from Object import Object

class Creature(Object):
	speed = 4
	seeInvisibleAbility = 0

	def __init__(self):
		Object.__init__(self)
		self.layer = 75
		self.name = "Python Creature"
		self.sprite = "human"
		self.AddComponent("Control")
