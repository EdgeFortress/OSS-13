from Engine.Server import Player
from Objects.Creatures.Ghost import Ghostize

def OnPlayerJoined(player: Player):
	print(player.ckey + " has joined! Yay!")
	player.AddVerb("ghost", Ghostize)
