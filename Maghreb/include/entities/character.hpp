#pragma once

#include "entity.hpp"
#include "stats_component.hpp"
#include "objects/equipment.hpp"
#include "main/animator.hpp"
#include "ai/movement_controller.hpp"
#include "ai/direction.hpp"
#include "main/animation_state.hpp"

class Character : public Entity {
public:
	explicit Character(SharedContext& context);
	~Character() override = default;

	// Компоненты
	StatsComponent& getStats();
	Equipment& getEquipment();
	Animator& getAnimator();
	MovementController& getMovement();

	// Боевой интерфейс
	virtual int getAttack() const;
	virtual int getDefense() const;
	virtual void takeDamage(int amount);

	// Состояние
	bool isAttacking() const;
	bool isMoving() const;
	bool isDying() const;
	bool isDead() const;
	bool isTakingDamage() const;
	bool isGuarding() const;
	bool isInvincible() const;

	void setAttacking(bool v);
	void setDying(bool v);
	void setInvincible(bool v);
	void setGuarding(bool v);

	// Движение
	Direction getDirection() const;
	void setDirection(Direction dir);
	sf::Vector2f getOffset() const;

	// Жизненный цикл
	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) override;

	// Утилиты
	int getRow() const;
	int getCol() const;

protected:
	StatsComponent m_stats;
	Equipment m_equipment;
	Animator m_animator;
	MovementController m_movement;

	// Состояние
	bool m_attacking = false;
	bool m_dying = false;
	bool m_deceased = false;
	bool m_takeDamage = false;
	bool m_guarding = false;
	bool m_invincible = false;
	int m_invincibleCounter = 0;

	// Зона атаки
	sf::FloatRect m_attackZone;

	// Индекс цели
	int m_indexEntity = 999;

	void updateInvincibilityTimer();
	AnimationState getAnimationRating() const;
};

