#pragma once

#include <list>

#include "Entry.hpp"
#include "CustomWidget.h"
#include "Shared/Types.hpp"

class Container : public CustomWidget {
public:
	explicit Container(const uf::vec2i &size = {});

	void Update(sf::Time timeElapsed) override;
	bool HandleEvent(sf::Event event) final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) override;
	bool OnMouseMoved(uf::vec2i position) override;
	bool OnMouseLeft() override;
	bool OnKeyPressed(sf::Event::KeyEvent keyEvent) override;
	bool OnTextEntered(uint32_t unicodeChar) override;

	void AddItem(CustomWidget *, const uf::vec2i &position);
	void Clear();

	bool SetActive(bool) override final;

protected:
	virtual void drawContainer() const;

private:
	void draw() const override;

private:
	std::list<uptr<CustomWidget>> items;

	std::list<uptr<CustomWidget>>::iterator curInputWidgetIterator;
	CustomWidget *underCursorWidget;
};
