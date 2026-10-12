#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Entity;

class InventoryComponent {
public:
	InventoryComponent();

	bool addItem(Entity* item);
	bool removeItem(int index);
	Entity* getItem(int index) const;

	int getCapacity() const;
	int getUsedSlots() const;

	void setCapacity(int capacity);

private:
	std::vector<Entity*> m_items;
	int m_capacity = 30;

	bool canFit(Entity* item) const;
};

