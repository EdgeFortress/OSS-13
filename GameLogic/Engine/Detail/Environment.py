def GetInheritors(klass):
	subclasses = set()
	subclasses.add(klass)
	work = [klass]
	while work:
		parent = work.pop()
		for child in parent.__subclasses__():
			if child not in subclasses:
				subclasses.add(child)
				work.append(child)
	return subclasses
