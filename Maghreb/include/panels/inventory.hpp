#pragma once

#include "main/shared_context.hpp"
#include "entities/entity.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

class Player;

class Inventory {
public:
	struct Cell {
		Cell() = default;
		Cell(int x, int y) : m_cellRect(x, y, 28, 29) {}
		Cell(sf::Vector2i pos) : m_cellRect(pos.x, pos.y, 28, 29) {}

		sf::IntRect m_cellRect;
		bool m_free = true;
	};

	explicit Inventory(SharedContext& context);
	~Inventory();

	void onCreate();

	// Поиск по типу
	bool findWeapon(EntityType type) const;
	bool findCloseCombatWeapons(EntityType type) const;
	bool findArmor(EntityType type) const;

	// Первоначальное заполнение
	void setItems();
	bool setInventory(Entity* entity);
	bool setCurrentItems(Entity* entity);
	bool setCurrentEquipment(Entity* entity, Cell* cell);
	bool checkCorrespondenceCells(Entity* entity, Cell* cell) const;
	bool canPlaceItem(Entity* entity) const;
	void setTwoHandedWeapons(Entity* entity, bool add);

	// Размещение в ручном режиме
	bool manageCurrentItems(Entity* entity);
	bool addToCell(Entity* entity, Cell* cell);
	bool releaseItem(Entity* entity, Cell* cell);
	bool addToSharedCell(Entity* entity);
	bool autoAdditionToSharedCells(Entity* entity);
	void rearrangeItem(Entity* entity);

	// Использование и информация
	void getInfoIP();
	void useItemIP();
	void setupInventoryIP();
	void storageAccess() {}

	// Удаление изношенных предметов
	void deleteUsedItems();

	void update();

	// Шаблон: ключ по значению
	template<typename K, typename V>
	Entity* getKey(std::unordered_map<Entity*, V>& map, V value) {
		for (auto&[key, val] : map) {
			if (val == value) return key;
		}
		return nullptr;
	}

	// Данные
	std::unordered_map<Entity*, sf::IntRect> m_inventoryPanelMap;
	std::unordered_map<Entity*, sf::IntRect> m_controlPanelMap;

	int m_maxRowIP = 4;
	int m_maxColIP = 10;
	std::vector<std::vector<Cell*>> m_cellIP;

	int m_maxColCP = 8;
	std::vector<Cell*> m_cellCP;

	bool m_largeCursorInstalled = false;

protected:
	Cell* m_weaponsCell = nullptr;
	Cell* m_shieldCell = nullptr;
	Cell* m_helmetCell = nullptr;
	Cell* m_armorCell = nullptr;
	Cell* m_amuletCell = nullptr;
	Cell* m_leftRingCell = nullptr;
	Cell* m_rightRingCell = nullptr;

private:
	void createCellsCP();
	void createCellsIP();
	void drawLargeCursor(Entity& entity) {}

	SharedContext& m_context;
	sf::Vector2i m_screenCoord{ 0, 0 };
	int m_cycleCounter = 0;

	sf::Font m_font;
	sf::Text m_information;
};

