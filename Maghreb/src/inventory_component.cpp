#include "panels/inventory_component.hpp"
#include <iostream>

InventoryComponent::InventoryComponent() {
	std::cerr << "[Stub] InventoryComponent created\n";
}

bool InventoryComponent::addItem(Entity* /*item*/) {
	std::cerr << "[Stub] InventoryComponent::addItem\n";
	return true;
}

bool InventoryComponent::removeItem(int /*index*/) {
	return false;
}

Entity* InventoryComponent::getItem(int /*index*/) const {
	return nullptr;
}

int InventoryComponent::getCapacity() const { return m_capacity; }
int InventoryComponent::getUsedSlots() const { return static_cast<int>(m_items.size()); }
void InventoryComponent::setCapacity(int capacity) { m_capacity = capacity; }

bool InventoryComponent::canFit(Entity* /*item*/) const {
	return static_cast<int>(m_items.size()) < m_capacity;
}
