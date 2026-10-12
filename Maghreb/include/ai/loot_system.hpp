#pragma once

#include <vector>
#include <SFML/System.hpp>

class Entity;
class SharedContext;

class LootSystem {
public:
	LootSystem();

	// Генерирует дроп для убитого врага
	std::vector<Entity*> generateLoot(SharedContext& context, const sf::Vector2f& position);

	// Добавляет предмет в мир
	void dropItem(SharedContext& context, Entity* item, const sf::Vector2f& position);

private:
	// Таблица вероятностей дропа
	struct DropEntry {
		int minRoll;
		int maxRoll;
		int itemType; // заглушка, потом заменится на enum
	};

	std::vector<DropEntry> m_dropTable;
};

