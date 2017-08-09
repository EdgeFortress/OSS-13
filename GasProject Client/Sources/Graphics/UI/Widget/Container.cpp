#include "Container.hpp"

#include <typeindex>
#include <typeinfo>

#include "Shared/Geometry/Vec2.hpp"
#include "Client.hpp"
#include "Entry.hpp"

Container::Container(sf::Vector2f size) :
    Widget(size), backgroundColor(sf::Color::Transparent), curInputEntry(nullptr)
	{ }

void Container::Update(sf::Time timeElapsed) { 
	for (auto &item : items) 
		item->Update(timeElapsed); 
}

bool Container::HandleEvent(sf::Event event) {
	switch (event.type) {
	    case sf::Event::MouseButtonPressed: {
            for (auto &item : items)
                if (std::type_index(typeid(*item.get())) == std::type_index(typeid(Entry)))
                    reinterpret_cast<Entry *>(item.get())->LoseFocus();
		    //uf::vec2i mousePosition = uf::vec2i(event.mouseButton.x, event.mouseButton.y);
		    //if (mousePosition < GetAbsPosition() || mousePosition >= GetAbsPosition() + GetSize())
			    //return true;
		    break;
	    }
	    case sf::Event::MouseMoved: {
		    //uf::vec2i mousePosition = uf::vec2i(event.mouseMove.x, event.mouseMove.y);
		    //if (mousePosition < GetAbsPosition() && mousePosition >= GetAbsPosition() + GetSize())
			   // return true;
		    break;
	    }
        case sf::Event::KeyPressed: {
            if (event.key.code == sf::Keyboard::Tab) {
                for (auto &item : items)
                    if (item.get() != curInputEntry && std::type_index(typeid(*item.get())) == std::type_index(typeid(Entry))) {
                        curInputEntry->LoseFocus();
                        curInputEntry = reinterpret_cast<Entry *>(item.get());
                        curInputEntry->GrabFocus();
                        break;
                    }
                return true;
            }
            break;
        }
	}

    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved) {
        for (auto &item : items)
            item->HandleEvent(event);
    } else {
        if (!(curInputEntry->HandleEvent(event))) {
            for (auto &item : items)
                if (item->HandleEvent(event)) {
                    curInputEntry = reinterpret_cast<Entry *>(item.get());
                    return true;
                }
        } else
            return true;
    }
    return false;
}

void Container::draw() const {
	sf::RenderTexture &buffer = const_cast<sf::RenderTexture &>(this->buffer);
	buffer.clear(backgroundColor);
    for (auto &item : items)
        buffer.draw(*(item.get()));
	buffer.display();
}

void Container::AddItem(Widget *widget, sf::Vector2f position) {
	items.push_back(uptr<Widget>(widget));
    if (curInputEntry == nullptr && (std::type_index(typeid(*widget)) == std::type_index(typeid(Entry))))
        curInputEntry = reinterpret_cast<Entry *>(widget);
	widget->SetPosition(position);
    widget->setParent(this);
}

void Container::Clear() {
    items.erase(items.begin(), items.end());
}

void Container::SetBackground(sf::Color color) {
    backgroundColor = color;
}
