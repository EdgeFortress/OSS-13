from Engine import CreateObject
from Objects.Creature import Creature

class Human(Creature):
	def __init__(self):
		super().__init__()
		self.name = "Python Human"
		self.sprite = "human"
		self.density = True
		self.PutOn(CreateObject("Objects.Items.Clothes.Uniform", None))
