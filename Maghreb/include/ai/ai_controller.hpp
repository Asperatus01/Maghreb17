#pragma once

#include <SFML/System.hpp>
#include "direction.hpp"

class Enemy;
class SharedContext;

class AiController {
public:
	enum class Behavior {
		Idle,
		Chase,
		Attack,
		ReturnHome,
		Flee
	};

	AiController();

	void update(Enemy& enemy, sf::Time dt);

	void setBehavior(Behavior behavior);
	Behavior getBehavior() const;

	void setDetectDistance(int dist);
	void setAttackDistance(int dist);

private:
	Behavior m_behavior = Behavior::Idle;
	int m_detectDistance = 16;
	int m_attackDistance = 1;

	void decideBehavior(Enemy& enemy);
	void executeChase(Enemy& enemy);
	void executeAttack(Enemy& enemy);
	void executeReturn(Enemy& enemy);
};

