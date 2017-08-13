#pragma once

#include <list>

#include "Entry.hpp"
#include "Widget.hpp"
#include "Shared/Types.hpp"

class Container : public Widget {
private:
	std::list<uptr<Widget>> items;
    sf::Color backgroundColor;

	std::list<uptr<Widget>>::iterator curInputWidgetIterator;

protected:
	virtual void draw() const override;

public:
	explicit Container(sf::Vector2f size = sf::Vector2f());

	virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

	void AddItem(Widget *, sf::Vector2f position);
    void Clear();

    void SetBackground(sf::Color);
	virtual bool SetActive(bool) final;
};