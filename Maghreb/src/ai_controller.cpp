#include "ai/ai_controller.hpp"
#include <iostream>

AiController::AiController() {
	std::cerr << "[Stub] AiController created\n";
}

void AiController::update(Enemy& /*enemy*/, sf::Time /*dt*/) {
	// Заглушка: ИИ логика вернётся позже
}

void AiController::setBehavior(Behavior behavior) { m_behavior = behavior; }
AiController::Behavior AiController::getBehavior() const { return m_behavior; }
void AiController::setDetectDistance(int dist) { m_detectDistance = dist; }
void AiController::setAttackDistance(int dist) { m_attackDistance = dist; }

void AiController::decideBehavior(Enemy& /*enemy*/) {}
void AiController::executeChase(Enemy& /*enemy*/) {}
void AiController::executeAttack(Enemy& /*enemy*/) {}
void AiController::executeReturn(Enemy& /*enemy*/) {}
