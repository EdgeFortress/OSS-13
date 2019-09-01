from Engine import CreateObject, ControlUIElement, Vec2i
from Objects.Creature import Creature

class Human(Creature):
	def __init__(self):
		super().__init__()
		self.name = "Python Human"
		self.sprite = "human"
		self.density = True
		self.PutOn(CreateObject("Objects.Items.Clothes.Uniform", None))

	def DefineUI(self, ui):
		element = ControlUIElement()
		element.position = Vec2i(100, 100)
		element.AddIcon("lhand")
		ui.UpdateElement("lhand", element)
