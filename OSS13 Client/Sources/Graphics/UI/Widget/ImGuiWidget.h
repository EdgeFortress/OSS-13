#pragma once

#include <Graphics/UI/Widget/Widget.hpp>

class ImGuiWidget : public Widget {
public:
	virtual void Update(sf::Time timeElapsed) = 0;
	virtual bool HandleEvent(sf::Event event) final;

protected:
	virtual void draw() const final;

	// Inherited via Widget
	virtual void Draw(sf::RenderTarget & target) const override;
	virtual void Hide() override;
	virtual void Show() override;
	virtual void SetPosition(const uf::vec2i &) override;
	virtual void SetPosition(const float x, const float y) override;
	virtual void SetSize(const uf::vec2i &) override;
	virtual uf::vec2i GetPosition() const override;
	virtual uf::vec2i GetAbsPosition() const override;
	virtual uf::vec2i GetSize() const override;
	virtual bool IsVisible() const override;
};
