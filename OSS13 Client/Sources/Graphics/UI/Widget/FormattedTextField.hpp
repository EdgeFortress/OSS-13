#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "CustomWidget.h"

class FormattedTextField : public CustomWidget {
public:
	FormattedTextField(const sf::Vector2f &size = sf::Vector2f());

	virtual void Update(sf::Time timeElapsed) final;
	virtual void SetSize(const sf::Vector2f &) final;

	bool OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) final;
	bool OnKeyPressed(sf::Event::KeyEvent keyEvent) final;

	struct Inscription
	{
		std::wstring stringText;
		std::vector<std::vector<sf::Text>> text;

		Inscription(const std::wstring &stringText) : stringText(stringText) { }
	};

	void AddInscription(const std::string &inscription);

protected:
	virtual void draw() const final;

private:
	float textXShift;
	int scrolled;

	bool resized;

	// TODO: what is it? mutable is needed here?
	mutable std::vector<Inscription> text;

	void scrollUp();
	void scrollDown();

	void parse(Inscription &, sf::Text &);
};
