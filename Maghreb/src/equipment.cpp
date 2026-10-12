#include "objects/equipment.hpp"

bool Equipment::isTwoHanded() const {
	// Заглушка: пока считаем, что всё одноручное
	return false;
}

void Equipment::clear() {
	m_weapon = nullptr;
	m_magic = nullptr;
	m_shield = nullptr;
	m_helmet = nullptr;
	m_armor = nullptr;
	m_amulet = nullptr;
	m_leftRing = nullptr;
	m_rightRing = nullptr;
}
