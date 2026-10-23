#include "objects/column.hpp" 
#include <iostream>


void Column::setPosition(const sf::Vector2f& pos) {
	// Двигаем СВОЙ спрайт (он не const, поэтому setPosition разрешён)
	m_staticSprite.setPosition(pos);
	// Обновляем позицию в базовом классе (для хитбоксов, коллизий и камеры)
	Entity::setPosition(pos);
}

void Column::setPosition(float x, float y) {
	m_staticSprite.setPosition(x, y);
	Entity::setPosition(x, y);
}