#include "Widget.hpp"

Widget::Widget(uf::vec2i size) : 
	isClosed(false) 
{ }

void Widget::Close() {
	isClosed = true;
}

bool Widget::IsClosed() {
	return isClosed;
}
