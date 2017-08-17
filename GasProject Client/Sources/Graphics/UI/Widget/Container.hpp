#pragma once

#include <list>

#include "Entry.hpp"
#include "Widget.hpp"
#include "Shared/Types.hpp"

class Container : public Widget {
public:
    explicit Container(sf::Vector2f size = sf::Vector2f());

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

    void AddItem(Widget *, sf::Vector2f position);
    void Clear();

    virtual bool SetActive(bool) final;

protected:
    virtual void draw() const override;

private:
	std::list<uptr<Widget>> items;

	std::list<uptr<Widget>>::iterator curInputWidgetIterator;
	Widget *underCursorWidget;
};