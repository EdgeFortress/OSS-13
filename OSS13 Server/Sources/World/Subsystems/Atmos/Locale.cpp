#include "Locale.h"

#include <Shared/Log.h>

using namespace subsystem::atmos;

Locale::Locale(ILocaleFactory *factory, IAtmosTile *tile) :
	factory(factory),
	graph(std::make_unique<Graph<IAtmosTile *>>())
{
	EXPECT(factory);
	EXPECT(tile);

	graph->AddNode(tile);
	tile->SetLocale(this);

	LOGD_(LogType::File) << "Locale " << ID() << ": created. First tile is " << tile->GetPos().toString();
}

Locale::Locale(ILocaleFactory *factory, std::unique_ptr<Graph<IAtmosTile *>> &&graph) :
	factory(factory),
	graph(std::move(graph))
{
	EXPECT(factory);
	EXPECT(this->graph->Count());

	for (auto tile : *(this->graph)) {
		EXPECT(!tile->GetLocale());
		tile->SetLocale(this);
	}

	LOGD_(LogType::File) << "Locale " << ID() << ": created from graph with " << this->graph->Count() << " nodes.";
}

Locale::~Locale() {
	LOGD_(LogType::File) << "Locale " << ID() << ": deleted";
}

void Locale::Update(std::chrono::microseconds /* timeElapsed */) {
	if (connectivityCheckIsNeeded) {
		divideByConnectedComponents();
		connectivityCheckIsNeeded = false;
	}
}

void Locale::divideByConnectedComponents() {
	auto connectedComponents = graph->GetConnectedComponents();
	EXPECT(!connectedComponents.empty());
	if (connectedComponents.size() == 1)
		return;
	for (auto iter = ++connectedComponents.begin(); iter != connectedComponents.end(); iter++) {
		auto &component = *iter;

		for (auto tile : *component)
			RemoveTile(tile);

		factory->CreateLocale(std::move(component));
	}
}

void Locale::AddConnection(IAtmosTile *first, IAtmosTile *second) {
	AddConnectionWithCheck(first, second, true);
}

void Locale::AddConnectionWithCheck(IAtmosTile *first, IAtmosTile *second, bool checkHasAtLeastOneTile) {
	LOGD_(LogType::File) << "Locale " << ID() << ": add connection " << first->GetPos().toString() << " with " << second->GetPos().toString();

	bool hasAtLeastOneTile = false;
	for (auto &tile : { first, second }) {
		EXPECT(tile);
		if (graph->IsContain(tile))
			hasAtLeastOneTile = true;
		else {
			Locale *lastLocale = reinterpret_cast<Locale *>(tile->GetLocale());

			if (lastLocale)
				merge(lastLocale);
			else
				addTile(tile);
		}
	}
	EXPECT(!checkHasAtLeastOneTile || hasAtLeastOneTile);
	graph->AddConnection(first, second);
}

void Locale::addTile(IAtmosTile *tile) {
	LOGD_(LogType::File) << "Locale " << ID() << ": add tile " << tile->GetPos().toString();
	EXPECT(!tile->GetLocale());
	graph->AddNode(tile);
	tile->SetLocale(this);
}

void Locale::merge(Locale *locale) {
	LOGD_(LogType::File) << "Locale " << ID() << ": merge with locale " << locale->ID();

	for (auto tile : *locale->graph)
		tile->SetLocale(this);

	graph->CopyIn(*locale->graph);
	locale->graph->Clear();
}

void Locale::RemoveTile(IAtmosTile *tileToRemove) {
	if (!tileToRemove->GetLocale())
		return;

	LOGD_(LogType::File) << "Locale " << ID() << ": remove tile " << tileToRemove->GetPos().toString();

	size_t connectionsCount = graph->CountConnections();

	graph->EraseNode(tileToRemove);

	size_t removedConnectionsCount = connectionsCount - graph->CountConnections();
	if (removedConnectionsCount > 1) {
		connectivityCheckIsNeeded = true;
	}

	tileToRemove->SetLocale(nullptr);
}

size_t Locale::GetSize() const {
	return graph->Count();
}
