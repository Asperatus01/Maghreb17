#include "objects/broken_column.hpp" 
#include <iostream>

void BrokenColumn::setPosition(const sf::Vector2f& pos) {
	m_staticSprite.setPosition(pos);
	Entity::setPosition(pos);
}

void BrokenColumn::setPosition(float x, float y) {
	m_staticSprite.setPosition(x, y);
	Entity::setPosition(x, y);
}
