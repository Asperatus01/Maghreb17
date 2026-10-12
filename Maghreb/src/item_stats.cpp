#include "objects/item_stats.hpp"
#include <cstdlib>
#include <iostream>

int ItemStats::getAttackValue() const {
	if (m_maxDamage <= m_minDamage) return m_minDamage;
	return m_minDamage + (std::rand() % (m_maxDamage - m_minDamage + 1));
}

int ItemStats::getDefenseValue() const {
	if (m_maxDefense <= m_minDefense) return m_minDefense;
	return m_minDefense + (std::rand() % (m_maxDefense - m_minDefense + 1));
}
