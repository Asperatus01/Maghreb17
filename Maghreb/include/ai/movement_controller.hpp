#pragma once

#include <SFML/System.hpp>
#include "direction.hpp"

class Character;
class SharedContext;

class MovementController {
public:
	MovementController();

	void update(Character& character, sf::Time dt);

	void setKnockBack(bool knock, float power);
	void setOnPath(bool onPath);
	void setGoal(const sf::Vector2i& goal);

	Direction getDirection() const;
	void setDirection(Direction dir);

	sf::Vector2f getOffset() const;
	bool isMoving() const;

private:
	Direction m_direction = Direction::Down;
	Direction m_prevDir = Direction::Nothing;
	sf::Vector2f m_offset;
	sf::Vector2i m_goal;

	bool m_knockBack = false;
	int  m_knockBackCounter = 0;
	bool m_onPath = false;
	bool m_movement = false;

	void handleKnockBack(Character& character, sf::Time dt, sf::Vector2f& movement);
	void handlePathfinding(Character& character, sf::Time dt, sf::Vector2f& movement);
	Direction searchPath(Character& character, int goalRow, int goalCol);
	void stopMovement(Character& character);
};

