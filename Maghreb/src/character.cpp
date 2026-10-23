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
bool Character::setEquipment(Entity*& slot, Entity* newItem) {
	if (slot == newItem) {
		return true; // Уже экипировано
	}
	slot = newItem; // Меняем сам указатель в слоте
	return true;
}

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

// Новый метод: инкапсулируем доступ к аниматору
void Character::activateAnimation(const std::string& name) {
	if (m_context.m_animHolderPtr) {
		m_context.m_animHolderPtr->setActiveAnimation(name);
	}
}

// Новый метод: даём доступ к AnimationHolder
// Возвращаем указатель, потому что он может быть nullptr
AnimationHolder* Character::getAnimatorHolder() const {
	return m_context.m_animHolderPtr;
}

void Character::update(sf::Time dt) {
	if (m_dying) {
		// Заглушка: обработка анимации смерти
		return;
	}
	m_movement.update(*this, dt);
	updateInvincibilityTimer();
}

//void Character::draw(sf::RenderTarget& /*target*/) {
//	// Заглушка: отрисовка через Animator
//}

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

// Функция нужна для смены инвентаря в Player
Entity*& Character::getSlotForType(EntityType type) {
	// Сначала проверяем, является ли тип оружием (чтобы не дублировать условия)
	auto isWeapon = [](EntityType t) {
		return t == EntityType::type_sword ||
			t == EntityType::type_axe ||
			t == EntityType::type_bow ||
			t == EntityType::type_mace ||
			t == EntityType::type_staff;
	};

	auto isArmor = [](EntityType t) {
		return t == EntityType::type_lightArmor ||
			t == EntityType::type_middleArmor ||
			t == EntityType::type_heavyArmor;
	};

	if (isWeapon(type)) {
		return m_equipment.m_weapon;
	}
	if (type == EntityType::type_shield) {
		return m_equipment.m_shield;
	}
	if (type == EntityType::type_helmet) {
		return m_equipment.m_helmet;
	}
	if (isArmor(type)) {
		return m_equipment.m_armor;
	}
	if (type == EntityType::type_amulet) {
		return m_equipment.m_amulet;
	}
	if (type == EntityType::type_ring) {
		// Для колец логика чуть сложнее: кладём в первый свободный
		if (!m_equipment.m_leftRing) {
			return m_equipment.m_leftRing;
		}
		return m_equipment.m_rightRing;
	}

	// Если тип не распознан — возвращаем ссылку на оружие (или можно кинуть исключение / assert)
	// Для безопасности лучше вернуть какой-то дефолтный слот, чтобы код не падал
	return m_equipment.m_weapon;
}

