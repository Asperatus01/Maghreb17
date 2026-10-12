#include "ai/loot_system.hpp"
#include <iostream>

LootSystem::LootSystem() {
	std::cerr << "[Stub] LootSystem created\n";
}

std::vector<Entity*> LootSystem::generateLoot(SharedContext& /*context*/, const sf::Vector2f& /*position*/) {
	// Заглушка: возвращаем пустой список
	return {};
}

void LootSystem::dropItem(SharedContext& /*context*/, Entity* /*item*/, const sf::Vector2f& /*position*/) {
	std::cerr << "[Stub] LootSystem::dropItem\n";
}
