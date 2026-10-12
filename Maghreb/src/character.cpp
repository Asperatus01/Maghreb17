#include "entities/character.hpp"
#include <iostream>

Character::Character(SharedContext& context)
	: Entity(context)
{
	std::cerr << "[Stub] Character created\n";
	m_type = EntityType::type_monster;
}

StatsComponent& Character::getStats() { return m_stats; }
Equipment& Character::getEquipment() { return m_equipment; }
Animator& Character::getAnimator() { return m_animator; }
MovementController& Character::getMovement() { return m_movement; }

int Character::getAttack() const { return m_stats.m_attack; }
int Character::getDefense() const { return m_stats.m_defense; }

void Character::takeDamage(int amount) {
	if (m_invincible) return;
	int damage = amount - m_stats.m_defense;
	if (damage < 0) damage = 0;
	m_stats.m_life -= damage;
	m_takeDamage = true;
	m_invincible = true;
	if (m_stats.m_life <= 0) {
		m_stats.m_life = 0;
		m_dying = true;
	}
}

bool Character::isAttacking() const { return m_attacking; }
bool Character::isMoving() const { return m_movement.isMoving(); }
bool Character::isDying() const { return m_dying; }
bool Character::isDead() const { return m_deceased; }
bool Character::isTakingDamage() const { return m_takeDamage; }
bool Character::isGuarding() const { return m_guarding; }
bool Character::isInvincible() const { return m_invincible; }

void Character::setAttacking(bool v) { m_attacking = v; }
void Character::setDying(bool v) { m_dying = v; }
void Character::setInvincible(bool v) { m_invincible = v; }
void Character::setGuarding(bool v) { m_guarding = v; }

Direction Character::getDirection() const { return m_movement.getDirection(); }
void Character::setDirection(Direction dir) { m_movement.setDirection(dir); }
sf::Vector2f Character::getOffset() const { return m_movement.getOffset(); }

void Character::update(sf::Time dt) {
	if (m_dying) {
		// Заглушка: обработка анимации смерти
		return;
	}
	m_movement.update(*this, dt);
	updateInvincibilityTimer();
}

void Character::draw(sf::RenderTarget& /*target*/) {
	// Заглушка: отрисовка через Animator
}

int Character::getRow() const {
	return static_cast<int>((m_position.y + m_solidArea.top) / 32);
}

int Character::getCol() const {
	return static_cast<int>((m_position.x + m_solidArea.left) / 32);
}

void Character::updateInvincibilityTimer() {
	if (m_invincible) {
		m_invincibleCounter++;
		if (m_invincibleCounter > 60) {
			m_invincible = false;
			m_invincibleCounter = 0;
		}
	}
}

AnimationState Character::getAnimationRating() const {
	if (m_dying) return AnimationState::Dying;
	if (m_takeDamage) return AnimationState::TakeDamage;
	if (m_guarding) return AnimationState::Guarding;
	if (m_attacking) return AnimationState::Attacking;
	if (m_movement.isMoving()) return AnimationState::Movement;
	return AnimationState::AttackingPose;
}
