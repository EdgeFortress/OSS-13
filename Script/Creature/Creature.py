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

		self.control = self.GetComponent("Control")

	def Update(self, timeElapsed):
		moveOrder = self.control.GetAndDropMoveOrder()
		if moveOrder: self.OnMoveOrder(moveOrder)

		moveZOrder = self.control.GetAndDropMoveZOrder()
		if moveZOrder: self.OnMoveZOrder(moveZOrder)
		

	def OnMoveOrder(self, order):
		self.Move(order)

	def OnMoveZOrder(self, order):
		self.MoveZ(order)
