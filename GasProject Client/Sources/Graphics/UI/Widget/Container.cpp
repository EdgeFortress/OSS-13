#include "Container.hpp"
#include "Client.hpp"

Container::Container(const sf::Vector2f &size) : Widget(size), backgroundColor(sf::Color::Transparent) { }

void Container::Hide() { for (auto &item : items) item.second->Hide(); }

void Container::Show() { for (auto &item : items) item.second->Show(); }

void Container::Update(sf::Time timeElapsed) { for (auto &item : items) item.second->Update(timeElapsed); }

void Container::HandleEvent(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int buttonX = event.mouseButton.x,
            buttonY = event.mouseButton.y;
        if (buttonX >= GetAbsPosition().x && buttonX <= GetAbsPosition().x + GetSize().x &&
            buttonY >= GetAbsPosition().y && buttonY <= GetAbsPosition().y + GetSize().y)
            for (auto &item : items)
                item.second->HandleEvent(event);
    } else if (event.type == sf::Event::MouseMoved) {
        int mouseX = event.mouseMove.x,
            mouseY = event.mouseMove.y;
        if (mouseX >= GetAbsPosition().x && mouseX <= GetAbsPosition().x + GetSize().x &&
            mouseY >= GetAbsPosition().y && mouseY <= GetAbsPosition().y + GetSize().y)
            for (auto &item : items)
                item.second->HandleEvent(event);
    } else
        for (auto &item : items)
            item.second->HandleEvent(event);
}

void Container::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(backgroundColor);
    for (auto &item : items) //item.second->Draw(buffer);
        buffer.draw(*(item.second.get()));
	buffer.display();
}

void Container::AddItem(Widget *widget, sf::Vector2f position) {
	items.push_back(std::pair<sf::Vector2f, uptr<Widget>>(position, uptr<Widget>(widget)));
	widget->SetPosition(position/* + GetPosition()*/);
    widget->SetParent(this);
}

void Container::SetPosition(const sf::Vector2f pos) {
	Widget::SetPosition(pos);
}
void Container::SetPosition(const float x, const float y) {
	Widget::SetPosition(x, y);
}

void Container::SetBackground(sf::Color color) {
    backgroundColor = color;
}