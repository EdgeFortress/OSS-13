import types

from Engine import CreateObject, ControlUIElement, Vec2i
from Objects.Creature import Creature

class Human(Creature):
	def __init__(self):
		super().__init__()
		self.name = "Python Human"
		self.sprite = "human"
		self.density = True
		self.PutOn(CreateObject("Objects.Items.Clothes.Uniform", None))

	def __uiFieldClicked(self, field):
		print(field.id + " clicked!")

	def DefineUI(self, ui):
		self.__uiFields = types.SimpleNamespace()

		element = ControlUIElement()
		element.id = "lhand"
		element.position = Vec2i(ui.center.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("lhand_inactive")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.lhand = element

		element = ControlUIElement()
		element.id = "rhand"
		element.position = Vec2i(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("rhand_inactive")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.rhand = element

		element = ControlUIElement()
		element.id = "equip"
		element.position = Vec2i(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("equip")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.equip = element

		element = ControlUIElement()
		element.id = "swap1"
		element.position = Vec2i(ui.center.x - ui.iconSize.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("swap1")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.swap1 = element

		element = ControlUIElement()
		element.id = "swap2"
		element.position = Vec2i(ui.center.x, ui.resolution.y - ui.iconSize.y * 2)
		element.AddIcon("swap2")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.swap2 = element

		element = ControlUIElement()
		element.id = "pocket1"
		element.position = Vec2i(ui.center.x + ui.iconSize.x, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("pocket")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.pocket1 = element

		element = ControlUIElement()
		element.id = "pocket2"
		element.position = Vec2i(ui.center.x + ui.iconSize.x * 2, ui.resolution.y - ui.iconSize.y)
		element.AddIcon("pocket")
		element.RegisterCallback(lambda element=element: self.__uiFieldClicked(element))
		ui.UpdateElement(element)
		self.__uiFields.pocket2 = element
