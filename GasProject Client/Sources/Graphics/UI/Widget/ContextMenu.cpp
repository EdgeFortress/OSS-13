#include "ContextMenu.hpp"

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"

ContextMenu::ContextMenu() {
    Hide();
	firstNotProcessedRow = content.end();
}

void ContextMenu::Update(sf::Time timeElapsed) {
	// If new rows
    if (firstNotProcessedRow != content.end()) {
		// Set new size
        uf::vec2u size;
        size.x = kWidth + 2 * kHorizontalMargin;
        size.y = uint(content.size()) * kRowHeight + kVerticalMargin;
        SetSize(size);

		if (underCursor_style.updated) {
			underCursor_rect.setFillColor(underCursor_style.backgroundColor);
			underCursor_rect.setOutlineColor(underCursor_style.backgroundColor);
			underCursor_rect.setSize(uf::vec2f(GetSize().x, kRowHeight));
			underCursor_style.updated = false;
		}

		// Create new texts
		uf::vec2i position = uf::vec2i(kHorizontalMargin, int(texts.size()) * kRowHeight + kVerticalMargin);
		for (; firstNotProcessedRow != content.end(); firstNotProcessedRow++) {
			sf::Text text = sf::Text(firstNotProcessedRow->string, *style.font, style.fontSize);
			text.setFillColor(style.textColor);
			text.setOutlineColor(style.textColor);
			text.setPosition(position);
			texts.push_back(std::move(text));
			position.y += kRowHeight;
		}
    }
}

bool ContextMenu::HandleEvent(sf::Event event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed: {
        if (event.mouseButton.button == sf::Mouse::Right) {
            if (!IsVisible()) Show();
            SetPosition(uf::vec2i(event.mouseButton.x, event.mouseButton.y));
            return true;
        }
    }
    case sf::Event::MouseMoved: {
        uf::vec2i mousePosition = uf::vec2i(event.mouseMove.x, event.mouseMove.y);
        if (mousePosition >= GetAbsPosition() && mousePosition < GetAbsPosition() + GetSize()) {
			mousePosition -= GetAbsPosition();
			uf::vec2i rectPos = uf::vec2i(0, mousePosition.y);
			rectPos.y -= kVerticalMargin;
			rectPos.y -= rectPos.y % kRowHeight;
			underCursor = true;
			underCursor_rect.setPosition(rectPos);
            return true;
        }
		return false;
    }
	case sf::Event::MouseLeft: {
		Hide();
		underCursor = false;
		return true;
	}
    }
    return false;
}

void ContextMenu::AddRow(const ContextMenuRow &row) {
    content.push_back(std::move(row));
	if (firstNotProcessedRow == content.end()) {
		firstNotProcessedRow--;
	}
}

void ContextMenu::SetUnderCursorStyle(const Style &style) {
	underCursor_style = style;
	underCursor_style.updated = true;
}

void ContextMenu::SetUnderCursorStyle(Style &&style) {
	underCursor_style = style;
	underCursor_style.updated = true;
}

Style &ContextMenu::GetUnderCursorStyle() {
	underCursor_style.updated = true;
	return underCursor_style;
}

void ContextMenu::draw() const {
    buffer.clear(style.backgroundColor);
	if (underCursor) buffer.draw(underCursor_rect);
    for (auto &text : texts) buffer.draw(text);
    buffer.display();
}

ContextMenuRow::ContextMenuRow(ContextMenuRow::Type type, const std::wstring &string) 
    : type(type), string(string)
{ }

void ContextMenuRow::SetFunction(std::function<void()> &) {

}

void ContextMenuRow::AddRow(ContextMenuRow &) {

}
