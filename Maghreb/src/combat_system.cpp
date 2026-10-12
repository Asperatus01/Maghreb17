#include "ai/combat_system.hpp"
#include <cmath>
#include <iostream>

CombatSystem::CombatSystem() {
	std::cerr << "[Stub] CombatSystem created\n";
}

sf::FloatRect CombatSystem::computeAttackZone(const Character& /*attacker*/) {
	return {};
}

int CombatSystem::checkHits(const Character& /*attacker*/, const std::vector<Entity*>& /*targets*/) {
	return -1;
}

int CombatSystem::calculateDamage(int attack, int defense, bool offBalance) {
	int damage = offBalance ? attack * 5 - defense : attack - defense;
	return damage > 0 ? damage : 0;
}

bool CombatSystem::canParry(const Character& /*defender*/, const Character& /*attacker*/) {
	return false; // Заглушка
}

Direction CombatSystem::getOppositeDirection(Direction dir) {
	switch (dir) {
	case Direction::Down:       return Direction::Up;
	case Direction::Downleft:   return Direction::Upright;
	case Direction::Left:       return Direction::Right;
	case Direction::Leftup:     return Direction::Rightdown;
	case Direction::Up:         return Direction::Down;
	case Direction::Upright:    return Direction::Downleft;
	case Direction::Right:      return Direction::Left;
	case Direction::Rightdown:  return Direction::Leftup;
	default:                    return Direction::Down;
	}
}

float CombatSystem::getAngle(float y, float x) {
	float angle = std::atan2(y, x) * 180.0f / 3.14159265358979f;
	angle = angle + std::ceil(-angle / 360.0f) * 360.0f;
	return angle;
}

Direction CombatSystem::getDirectionByAngle(float angle) {
	if (angle >= 0 && angle <= 22.5f)         return Direction::Right;
	else if (angle <= 67.5f)                 return Direction::Rightdown;
	else if (angle <= 112.5f)                return Direction::Down;
	else if (angle <= 157.5f)                return Direction::Downleft;
	else if (angle <= 202.5f)                return Direction::Left;
	else if (angle <= 247.5f)                return Direction::Leftup;
	else if (angle <= 292.5f)                return Direction::Up;
	else if (angle <= 337.5f)                return Direction::Upright;
	else                                      return Direction::Right;
}
