#pragma once

#include <list>
#include <string>
#include <functional>

#include "Widget.hpp"
#include "Shared/Types.hpp"

class ContextMenuRow;

class ContextMenu : public Widget {
public:
    explicit ContextMenu();
    virtual ~ContextMenu() = default;

    virtual void Update(sf::Time timeElapsed) final;
    virtual bool HandleEvent(sf::Event event) final;

    void AddRow(const ContextMenuRow &);

	void SetUnderCursorStyle(const Style &);
	void SetUnderCursorStyle(Style &&);

	Style &GetUnderCursorStyle();

protected:
    virtual void draw() const final;

private:
    const uint kWidth = 100;
    const uint kRowHeight = 20;
    const uint kVerticalMargin = 2;
	const uint kHorizontalMargin = 5;

    std::list<ContextMenuRow> content;
	std::list<sf::Text> texts;
	std::list<ContextMenuRow>::iterator firstNotProcessedRow;

	sf::RectangleShape underCursor_rect;
	bool underCursor;
	Style underCursor_style;
};

class ContextMenuRow {
public:
    enum class Type : char {
        FUNCTION,
        SUBMENU
    };

    ContextMenuRow(Type, const std::wstring &string);

    void SetFunction(std::function<void()> &);
    void AddRow(ContextMenuRow &);

    friend ContextMenu;

private:
    Type type;
    std::wstring string;

    // if FUNCTION
    std::function<void()> function;
    // if SUBMENU
    std::list<ContextMenuRow> content;
};