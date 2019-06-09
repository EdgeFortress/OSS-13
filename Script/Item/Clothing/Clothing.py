from Item.Item import Item

class Clothing(Item):
	def __init__(self):
		Item.__init__(self)
		self.name = "PythonClothing"
		self.sprite = "grey"
