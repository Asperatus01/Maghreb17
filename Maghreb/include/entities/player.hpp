#pragma once

#include "entities/character.hpp"
#include "panels/inventory_component.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Player : public Character {
public:
	explicit Player(SharedContext& context);
	~Player() override = default;

	// Боевая система
	int getAttack() const override;
	int getDefense() const override;

	void attack();
	void destroyObject();
	void pickUpObject(int index);
	void dialogNpc(int index);

	void damagePlayer(Character* attacker);
	void damageMonster(Character* target);
	void damageInteractiveObject(Character* target);

	bool getCloseCombatWeapon() const;
	bool findCloseCombatWeapons(EntityType type) const;
	int getAttackDistance() const;

	// Расстояния
	int getTileDistance(int index) const;
	int getTileDistanceToMouse() const;
	float getDistanceToEntity(int index) const;
	sf::Vector2f getCenterEntity(int index) const;

	// Прокачка
	void checkLevelUp();

	// Сообщения
	void setupMessage(sf::Font font, sf::Text::Style style,
		unsigned int size, sf::Color color);
	void addMessage(const std::wstring& text);
	void drawMessages(sf::RenderTarget& target, const sf::FloatRect& viewSpace);

	// Геттеры
	sf::Sprite& getSprite();
	InventoryComponent& getInventory();

	// Жизненный цикл
	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) const override;

	// Экипировка
	void equipItem(Entity* item);

private:
	// Компоненты
	InventoryComponent m_inventory;

	// Спрайт и анимация
	sf::Sprite m_sprite;

	// Сообщения
	sf::Font m_font;
	sf::Text m_message;
	std::vector<sf::Text> m_messageVect;
	std::vector<int> m_messageTimer;
	void updateMessages();

	// Игрок-специфичные флаги
	bool m_conventionalWeapons = true;
	bool m_amendment = false;
	bool m_shootArrow = false;
	int m_counter = 0;

	// Управление
	void handleInput(sf::Time dt);
	void handleKeyboardMovement(sf::Vector2f& movement);
	void handleGuardAndCounters();
	void handleArrowShooting();

	// Анимация
	void animationRanking(sf::Time dt);
	void animationControl();

	// Отладка
	void drawDebug(sf::RenderTarget& target);
	sf::RectangleShape m_rectCollision;
};

