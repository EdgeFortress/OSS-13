#include "ControlUI.h"

uf::vec2i ControlUIElement::GetPosition() const { return position; }
void ControlUIElement::SetPosition(uf::vec2i pos) {
	position = pos;
	updated = true;
}

void ControlUIElement::AddIcon(const std::string &icon) {
	icons.push_back(icon);
	updated = true;
}

void ControlUIElement::ClearIcons() {
	icons.clear();
	updated = true;
}
