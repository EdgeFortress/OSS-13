#include "ContextMenu.hpp"

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"

ContextMenu::ContextMenu() {
    Hide();
}

void ContextMenu::Update(sf::Time timeElapsed) {
    if (newRows) {
        uf::vec2u size;
        size.x = kWidth;
        size.y = uint(content.size()) * kRowHeight + 2 * kVerticalMargin;
        SetSize(size);
        newRows = false;
    }
}

bool ContextMenu::HandleEvent(sf::Event event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed: {
        if (event.mouseButton.button == sf::Mouse::Right) {
            if (IsVisible()) Hide();
            else Show();
            SetPosition(uf::vec2i(event.mouseButton.x, event.mouseButton.y));
            return true;
        }
    }
    }
    return false;
}

void ContextMenu::AddRow(const ContextMenuRow &row) {
    content.push_back(std::move(row));
    newRows = true;
}

void ContextMenu::SetFont(const sf::Font *font) {
    this->font = font;
}

void ContextMenu::draw() const {
    buffer.clear(sf::Color::Black);
    uf::vec2u pos = uf::vec2u(0, kVerticalMargin);
    for (auto &row : content) {
        sf::Text text = sf::Text(row.string, *font, 10);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::White);
        text.setPosition(pos);
        buffer.draw(text);
        pos.y += kRowHeight;
    }
    buffer.display();
}

ContextMenuRow::ContextMenuRow(ContextMenuRow::Type type, const std::wstring &string) 
    : type(type), string(string)
{ }

void ContextMenuRow::SetFunction(std::function<void()> &) {

}

void ContextMenuRow::AddRow(ContextMenuRow &) {

}
