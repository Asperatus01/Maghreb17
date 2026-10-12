#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "ai/direction.hpp"

class Character;
class Entity;

class CombatSystem {
public:
	CombatSystem();

	// Расчёт зоны атаки в зависимости от направления
	static sf::FloatRect computeAttackZone(const Character& attacker);

	// Проверка попадания по списку целей
	static int checkHits(const Character& attacker, const std::vector<Entity*>& targets);

	// Расчёт урона
	static int calculateDamage(int attack, int defense, bool offBalance);

	// Проверка парирования
	static bool canParry(const Character& defender, const Character& attacker);

	// Направление, противоположное данному
	static Direction getOppositeDirection(Direction dir);

	// Угол по координатам
	static float getAngle(float y, float x);

	// Направление по углу
	static Direction getDirectionByAngle(float angle);

private:
	static constexpr float kAttackAreaX = 50.f;
	static constexpr float kAttackAreaY = 50.f;
};

