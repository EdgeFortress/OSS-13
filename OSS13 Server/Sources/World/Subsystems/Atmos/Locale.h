#pragma once

#include <World/Subsystems/Atmos/ILocale.h>

#include "ILocaleFactory.h"

namespace subsystem::atmos {

class Locale : public ILocale {
public:
	Locale(ILocaleFactory *factory, IAtmosTile *tile);
	Locale(ILocaleFactory *factory, std::unique_ptr<Graph<IAtmosTile *>> &&graph);

	~Locale() override;

	void Update(std::chrono::microseconds timeElapsed);

	void AddConnection(IAtmosTile *first, IAtmosTile *second);
	void AddConnectionWithCheck(IAtmosTile *first, IAtmosTile *second, bool checkHasAtLeastOneTile);

	void RemoveTile(IAtmosTile *tile);

	size_t GetSize() const;

private:
	void addTile(IAtmosTile *tile);
	void merge(Locale *locale);
	void divideByConnectedComponents();

private:
	std::unique_ptr<Graph<IAtmosTile *>> graph;

	bool connectivityCheckIsNeeded{ false };
	ILocaleFactory *factory;
};

} // namespace subsystem::atmos
