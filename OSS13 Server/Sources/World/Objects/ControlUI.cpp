#include "ControlUI.h"

#include <Server.hpp>
#include <Player.hpp>
#include <World/Objects/Control.hpp>
#include <Resources/ResourceManager.hpp>

#include <Shared/Network/Protocol/ServerToClient/Commands.h>

void ControlUIElement::OnClick() {
	callback();
}

void ControlUIElement::RegistrateCallback(std::function<void()> callback) {
	this->callback = callback;
}

uf::vec2i ControlUIElement::GetPosition() const { return position; }
void ControlUIElement::SetPosition(uf::vec2i pos) {
	position = pos;
	NeedUpdate();
}

void ControlUIElement::AddIcon(const std::string &icon) {
	auto iconId = GServer->RM()->GetIconInfo(icon).id;
	spritesIds.push_back(iconId);
	NeedUpdate();
}

void ControlUIElement::PopIcon() {
	spritesIds.pop_back();
	NeedUpdate();
}

void ControlUIElement::ClearIcons() {
	spritesIds.clear();
	NeedUpdate();
}

void ControlUIElement::SetId(const std::string &id) { elementId = id; }
const std::string &ControlUIElement::GetId() const { return elementId; }

void ControlUIElement::SetControlUI(ControlUI *parent) { this->parent = parent; }
ControlUI *ControlUIElement::GetControlUI() const { return parent; }

void ControlUIElement::NeedUpdate() {
	if (parent)
		parent->NeedUpdate();
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
	NeedFullRefresh();
}

void ControlUI::Update(std::chrono::microseconds /*timeElapsed*/) {
	if (!updated)
		return;
	updated = false;

	auto command = std::make_unique<network::protocol::server::ControlUIUpdateCommand>();

	if (!synched)
		command->clear = true;

	for (auto &[key, element] : elements) {
		bool updated = element->GetAndDropUpdatedState();
		if (!synched || updated)
			command->elements.push_back(*element);
	}

	control->GetPlayer()->AddCommandToClient(command.release());
	synched = true;
}

void ControlUI::OnClick(const std::string &key) {
	auto clicked = elements.find(key);
	if (clicked != elements.end()) {
		clicked->second->OnClick();
	}
}

void ControlUI::UpdateElement(std::shared_ptr<ControlUIElement> element) {
	element->SetControlUI(this);
	elements[element->GetId()] = std::move(element);
	updated = true;
}

void ControlUI::RemoveElement(const std::string &key) {
	elements.erase(key);
	updated = true;
}

void ControlUI::NeedUpdate() {
	updated = true;
}

void ControlUI::NeedFullRefresh() {
	NeedUpdate();
	synched = false;
}

uf::vec2i ControlUI::GetResolution() const { return { Global::control_ui::SIDE_SIZE, Global::control_ui::SIDE_SIZE }; }
uf::vec2i ControlUI::GetCenter() const { return GetResolution() / 2; }
uf::vec2i ControlUI::GetIconSize() const { return { Global::control_ui::ITEM_SIZE, Global::control_ui::ITEM_SIZE }; }
