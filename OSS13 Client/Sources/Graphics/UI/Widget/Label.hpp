#pragma once

#include "CustomWidget.h"

class Label : public CustomWidget {
public:
	Label();
	Label(const sf::String &);
	
	virtual void Update(sf::Time timeElapsed) override;
	virtual bool HandleEvent(sf::Event event) override;

	void SetString(const sf::String &string);

protected:
    virtual void draw() const override;

private:
    sf::Text text;
};