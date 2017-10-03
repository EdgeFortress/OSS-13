#pragma once

#include <list>

#include "Entry.hpp"
#include "Widget.hpp"
#include "Shared/Types.hpp"

class Container : public Widget {
public:
    explicit Container(const uf::vec2i &size = {});

    void Update(sf::Time timeElapsed) override final;
    bool HandleEvent(sf::Event event) override final;

    void AddItem(Widget *, const uf::vec2i &position);
    void Clear();

    bool SetActive(bool) override final;

protected:
    void draw() const override final;

private:
	std::list<uptr<Widget>> items;

	std::list<uptr<Widget>>::iterator curInputWidgetIterator;
	Widget *underCursorWidget;
};