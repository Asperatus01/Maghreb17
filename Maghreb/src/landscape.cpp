#include "objects/landscape.hpp" // подключаем заголовок
#include <iostream>

// Реализация для Vector2f
void Tree::setPosition(const sf::Vector2f& pos) {
	// Двигаем реальный спрайт (у Tree есть прямой доступ, он не const)
	m_staticSprite.setPosition(pos);

	// Обновляем позицию для хитбоксов в базовом классе
	Entity::setPosition(pos);
}

// Реализация для float x, y
void Tree::setPosition(float x, float y) {
	m_staticSprite.setPosition(x, y);
	Entity::setPosition(x, y);
}
