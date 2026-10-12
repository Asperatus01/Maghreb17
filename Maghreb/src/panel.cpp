#include "panels/panel.hpp"
#include <iostream>

Panel::Panel(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] Panel created\n";
}

bool Panel::inBounds(const sf::IntRect& rect, const sf::Vector2i& point) const {
	return rect.contains(point);
}
