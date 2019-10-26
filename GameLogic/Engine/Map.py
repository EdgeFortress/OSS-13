from Engine.World import Map, CreateObject, Object
from Engine.Geometry import Vector
from Objects.Turfs.Airlock import Airlock

def FillMap(map):
	"""
	FillMap is called by Engine when map is created

	You can add any map generation logic here

	"""
	z = 0
	for i in range(45, 56):
		for j in range(45, 56):
			CreateObject("Objects.Turfs.Floor.Floor", map.GetTile(Vector(i, j, z)))
			if i < 52 or i > 53 or j < 47 or j > 48:
				CreateObject("Objects.Turfs.Ceiling.Ceiling", map.GetTile(Vector(i, j, z)))
			if i == 45 or i == 55 or j == 45 or j == 55:
				if i == 50 or j == 50:
					airlock = CreateObject("Objects.Turfs.Airlock.Airlock", map.GetTile(Vector(i, j, z)))
					if (i == 55 and j == 50) or (i == 50 and j == 55):
						airlock.Lock()
				else:
					CreateObject("Objects.Turfs.Wall.Wall", map.GetTile(Vector(i, j, z)))

	z = 1
	for i in range(50, 56):
		for j in range(45, 51):
			CreateObject("Objects.Turfs.Ceiling.Ceiling", map.GetTile(Vector(i, j, z)))
			if i < 52 or i > 53 or j < 47 or j > 48:
				CreateObject("Objects.Turfs.Floor.Floor", map.GetTile(Vector(i, j, z)))
			if i == 50 or i == 55 or j == 45 or j == 50:
				CreateObject("Objects.Turfs.Wall.Wall", map.GetTile(Vector(i, j, z)))

	CreateObject("Objects.Items.Taser.Taser", map.GetTile(Vector(47, 53, 0)))
	CreateObject("Objects.Items.Taser.Taser", map.GetTile(Vector(53, 47, 0)))
	CreateObject("Objects.Items.Clothes.Uniform.Clown", map.GetTile(Vector(46, 53, 1)))
	CreateObject("Objects.Items.Clothes.Uniform.Captain", map.GetTile(Vector(51, 50, 2)))
