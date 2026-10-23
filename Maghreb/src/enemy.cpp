#include "entities/enemy.hpp"
#include <iostream>

Enemy::Enemy(SharedContext& context)
	: Character(context)
{
	std::cerr << "[Stub] Enemy created\n";
	m_type = EntityType::type_monster;
}

AiController& Enemy::getAi() { return m_ai; }
LootSystem& Enemy::getLoot() { return m_loot; }

void Enemy::update(sf::Time dt) {
	if (m_dying) {
		// Заглушка: обработка смерти и возможного возрождения
		return;
	}
	m_ai.update(*this, dt);
	Character::update(dt);
}

void Enemy::draw(sf::RenderTarget& target) const{
	Character::draw(target);
}

void Enemy::setAction() {
	// Заглушка: делегируется в AiController при реализации
}
