#include "ControlUI.h"

#include <Player.hpp>
#include <World/Objects/Control.hpp>

#include <Shared/Network/Protocol/ServerToClient/Commands.h>

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

bool ControlUIElement::GetAndDropUpdatedState() {
	bool updated = this->updated;
	this->updated = false;
	return updated;
}

ControlUI::ControlUI(Control *control) :
	control(control) 
{
	EXPECT(control);
}

void ControlUI::Update(std::chrono::microseconds /*timeElapsed*/) {
	if (!updated)
		return;
	updated = false;

	auto command = std::make_unique<network::protocol::server::ControlUIUpdateCommand>();

	for (auto &[key, element] : elements) {
		bool updated = element->GetAndDropUpdatedState();
		if (updated)
			command->elements.push_back(*element);
	}

	control->GetPlayer()->AddCommandToClient(command.release());
}

void ControlUI::UpdateElement(const std::string &key, std::shared_ptr<ControlUIElement> element) {
	elements[key] = std::move(element);
	updated = true;
}

void ControlUI::RemoveElement(const std::string &key) {
	elements.erase(key);
	updated = true;
}
