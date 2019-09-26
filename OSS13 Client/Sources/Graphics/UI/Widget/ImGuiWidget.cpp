#include "ImGuiWidget.h"

bool ImGuiWidget::HandleEvent(sf::Event event) { return false; }
void ImGuiWidget::draw() const { }

void ImGuiWidget::Draw(sf::RenderTarget & target) const { }
void ImGuiWidget::Hide() { }
void ImGuiWidget::Show() { }
void ImGuiWidget::SetPosition(const uf::vec2i &) { }
void ImGuiWidget::SetPosition(const float x, const float y) { }
void ImGuiWidget::SetSize(const uf::vec2i &) { }
Widget *ImGuiWidget::GetParent() const { return nullptr; }
uf::vec2i ImGuiWidget::GetPosition() const { return {}; }
uf::vec2i ImGuiWidget::GetAbsolutePosition() const { return {}; }
uf::vec2i ImGuiWidget::GetSize() const { return {}; }
uf::vec2i ImGuiWidget::GetAbsoluteSize() const { return {}; }
uf::vec2f ImGuiWidget::GetScale() const { return {}; }
bool ImGuiWidget::IsVisible() const { return false; }
