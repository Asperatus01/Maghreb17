#include "entities/entity.hpp"
#include <iostream>

// Конструктор: принимает ссылку, инициализирует ссылку
Entity::Entity(SharedContext& context)
	: m_context(context)
	, m_position(0.f, 0.f)
	, m_solidArea(0.f, 0.f, 32.f, 32.f)
	, m_solidAreaDefault(m_solidArea)
	, m_type(EntityType::type_unknown)
	, m_collision(true)
	, m_name(L"")                 // <-- Важно: инициализируем пустым
	, m_itemStats()               // <-- std::optional по умолчанию пустой
{
	std::cerr << "[Stub] Entity created\n";
}

sf::FloatRect Entity::getWorldCoordSolid() const {
	return {
		m_position.x + m_solidArea.left,
		m_position.y + m_solidArea.top,
		m_solidArea.width,
		m_solidArea.height
	};
}

sf::Vector2f Entity::getCenter() const {
	return {
		m_position.x + m_solidArea.left + m_solidArea.width / 2.f,
		m_position.y + m_solidArea.top + m_solidArea.height / 2.f
	};
}

void Entity::update(sf::Time dt) {
	// Заглушка
}

void Entity::draw(sf::RenderTarget& /*target*/) {
	// Заглушка: отрисовку спрайта делаем в наследниках
}

void Entity::interact() {
	// Пустая реализация
}

void Entity::use() {
	// Пустая реализация
}
