import Engine.Server
from Objects.Creatures.Ghost import Ghostize

def OnPlayerJoined(player: Engine.Server.Player):
	"""is called by Engine on player join to game"""
	print(player.ckey + " has joined! Yay!")
	player.AddVerb("ghost", Ghostize)
