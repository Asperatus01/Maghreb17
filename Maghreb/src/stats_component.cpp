#include "entities/stats_component.hpp"
#include <iostream>

void StatsComponent::resetToDefaults() {
	std::cerr << "[Stub] StatsComponent::resetToDefaults\n";
}

void StatsComponent::applyLevelUp() {
	m_level++;
	m_nextLevelExp = 100 * (m_level + 1) * (m_level + 1) - m_experience;
	m_maxLife += 2;
	m_maxMana += 2;
	m_strength++;
	m_dexterity++;
	m_pointsToDistribute += 5;
	std::cerr << "[Stub] Level up to " << m_level << "\n";
}
