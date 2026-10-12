#include "ai/movement_controller.hpp"
#include <iostream>

MovementController::MovementController() {
	std::cerr << "[Stub] MovementController created\n";
}

void MovementController::update(Character& /*character*/, sf::Time /*dt*/) {
	// Заглушка: реальная логика движения вернётся позже
}

void MovementController::setKnockBack(bool knock, float power) {
	m_knockBack = knock;
	// power будет использована при реализации
}

void MovementController::setOnPath(bool onPath) { m_onPath = onPath; }
void MovementController::setGoal(const sf::Vector2i& goal) { m_goal = goal; }

Direction MovementController::getDirection() const { return m_direction; }
void MovementController::setDirection(Direction dir) { m_direction = dir; }

sf::Vector2f MovementController::getOffset() const { return m_offset; }
bool MovementController::isMoving() const { return m_movement; }

void MovementController::handleKnockBack(Character& /*character*/, sf::Time /*dt*/, sf::Vector2f& /*movement*/) {}
void MovementController::handlePathfinding(Character& /*character*/, sf::Time /*dt*/, sf::Vector2f& /*movement*/) {}
Direction MovementController::searchPath(Character& /*character*/, int /*goalRow*/, int /*goalCol*/) {
	return Direction::Nothing;
}
void MovementController::stopMovement(Character& /*character*/) {}
