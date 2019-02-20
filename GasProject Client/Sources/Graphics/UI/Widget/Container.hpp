#pragma once

#include <list>

#include "Entry.hpp"
#include "CustomWidget.h"
#include "Shared/Types.hpp"

class Container : public CustomWidget {
public:
    explicit Container(const uf::vec2i &size = {});

    void Update(sf::Time timeElapsed) override final;
    bool HandleEvent(sf::Event event) override final;

    void AddItem(CustomWidget *, const uf::vec2i &position);
    void Clear();

    bool SetActive(bool) override final;

protected:
    void draw() const override final;

private:
	std::list<uptr<CustomWidget>> items;

	std::list<uptr<CustomWidget>>::iterator curInputWidgetIterator;
	CustomWidget *underCursorWidget;
};