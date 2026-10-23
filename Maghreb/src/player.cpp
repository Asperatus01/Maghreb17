#include "entities/player.hpp"
#include <cmath>
#include <iostream>

static constexpr float kTileSize = 32.f;

Player::Player(SharedContext& context)
	: Character(context)
{
	std::cerr << "[Stub] Player created\n";

	m_type = EntityType::type_player;
	setName(L"Stephanie");
	setType(EntityType::type_player);

	// Базовые статы
	m_stats.m_speed = 70.f;
	m_stats.m_defaultSpeed = 70.f;
	m_stats.m_vitality = 10;
	m_stats.m_defaultVitality = 10;
	m_stats.m_maxLife = 10 + m_stats.m_vitality;
	m_stats.m_life = m_stats.m_maxLife;
	m_stats.m_maxMana = 30;
	m_stats.m_mana = m_stats.m_maxMana;
	m_stats.m_level = 1;
	m_stats.m_strength = 20;
	m_stats.m_defaultStrength = 20;
	m_stats.m_dexterity = 20;
	m_stats.m_defaultDexterity = 20;
	m_stats.m_magic = 20;
	m_stats.m_defaultMagic = 20;
	m_stats.m_pointsToDistribute = 5;
	m_stats.m_armorClass = 1;
	m_stats.m_accuracy = 100;
	m_stats.m_detDistance = 16;
	m_stats.m_talkDistance = 1;
	m_stats.m_experience = 0;
	m_stats.m_nextLevelExp = 100 * 2 * 2;

	m_solidAreaDefault = { 38.f, 40.f, 20.f, 20.f };
	m_solidArea = m_solidAreaDefault;

	m_position = sf::Vector2f(40.f * kTileSize, 39.f * kTileSize);
	m_sprite.setPosition(m_position);

	// Заглушки для атаки/защиты
	m_stats.m_attack = 10;
	m_stats.m_defense = 5;

	// Настройка сообщения
	m_message.setStyle(sf::Text::Italic);
	m_message.setCharacterSize(16);
	m_message.setFillColor(sf::Color::White);
}

int Player::getAttack() const {
	return m_stats.m_attack;
}

int Player::getDefense() const {
	return m_stats.m_defense;
}

void Player::attack() {
	(void)m_equipment;
}

void Player::destroyObject() {}

void Player::pickUpObject(int /*index*/) {}

void Player::dialogNpc(int /*index*/) {}

void Player::damagePlayer(Character* /*attacker*/) {}

void Player::damageMonster(Character* /*target*/) {}

void Player::damageInteractiveObject(Character* /*target*/) {}

bool Player::getCloseCombatWeapon() const {
	return false;
}

bool Player::findCloseCombatWeapons(EntityType /*type*/) const {
	return false;
}

int Player::getAttackDistance() const {
	return 1;
}

int Player::getTileDistance(int /*index*/) const {
	return 999;
}

int Player::getTileDistanceToMouse() const {
	return 0;
}

float Player::getDistanceToEntity(int /*index*/) const {
	return 999.f;
}

sf::Vector2f Player::getCenterEntity(int /*index*/) const {
	return { -1.f, -1.f };
}

void Player::checkLevelUp() {}

void Player::equipItem(Entity* item) {
	if (!item) {
		return; // Нельзя экипировать nullptr
	}

	EntityType type = item->getType();

	// Получаем ссылку на нужный слот по типу
	Entity*& targetSlot = getSlotForType(type);

	// Меняем слот через универсальный сеттер
	if (setEquipment(targetSlot, item)) {
		// Триггер предзагрузки
		//m_game->preloadWeaponPack(item->getAnimationType());

		// Обновляем визуал (спрайт, анимацию)
		//updateVisuals();
	}
}


void Player::setupMessage(sf::Font /*font*/, sf::Text::Style /*style*/,
	unsigned int /*size*/, sf::Color /*color*/) {}

void Player::addMessage(const std::wstring& text) {
	sf::Text msg = m_message;
	msg.setString(text);
	m_messageVect.push_back(msg);
	m_messageTimer.push_back(0);
}

void Player::drawMessages(sf::RenderTarget& target, const sf::FloatRect& viewSpace) {
	float messageX = viewSpace.left + 32.f;
	float messageY = viewSpace.top + 32.f * 5;

	for (size_t i = 0; i < m_messageVect.size(); ) {
		sf::Text& msg = m_messageVect[i];

		// Тень
		msg.setFillColor(sf::Color::Blue);
		msg.setPosition(messageX + 2.f, messageY + 2.f);
		target.draw(msg);

		// Текст
		msg.setFillColor(sf::Color::Cyan);
		msg.setPosition(messageX, messageY);
		target.draw(msg);

		m_messageTimer[i]++;
		messageY += 25.f;

		if (m_messageTimer[i] > 180) {
			m_messageVect.erase(m_messageVect.begin() + i);
			m_messageTimer.erase(m_messageTimer.begin() + i);
		}
		else {
			++i;
		}
	}
}

void Player::updateMessages() {}

sf::Sprite& Player::getSprite() {
	return m_sprite;
}

InventoryComponent& Player::getInventory() {
	return m_inventory;
}

void Player::update(sf::Time dt) {
	handleInput(dt);
	animationRanking(dt);
	updateMessages();
}

void Player::draw(sf::RenderTarget& target) const{
	target.draw(m_sprite);
	//drawDebug(target);
	// ИСПРАВЛЕНО: getViewport() без аргументов
	//drawMessages(target, target.getView().getViewport());
}

void Player::handleInput(sf::Time /*dt*/) {
	sf::Vector2f movement(0.f, 0.f);
	handleKeyboardMovement(movement);

	// Дебаг-клавиши
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) m_stats.m_life -= 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) m_stats.m_mana -= 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) m_guarding = true;

	// ИСПРАВЛЕНО: у StatsComponent нет m_offset, двигаем напрямую
	m_position.x += movement.x;
	m_position.y += movement.y;
	m_sprite.setPosition(m_position);

	handleGuardAndCounters();
	handleArrowShooting();
}

void Player::handleKeyboardMovement(sf::Vector2f& movement) {
	float speed = m_stats.m_speed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_movement.setDirection(Direction::Up);
		movement.y -= speed;
		m_movement.setOnPath(false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_movement.setDirection(Direction::Down);
		movement.y += speed;
		m_movement.setOnPath(false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_movement.setDirection(Direction::Left);
		movement.x -= speed;
		m_movement.setOnPath(false);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_movement.setDirection(Direction::Right);
		movement.x += speed;
		m_movement.setOnPath(false);
	}
}

void Player::handleGuardAndCounters() {
	updateInvincibilityTimer();
}

void Player::handleArrowShooting() {
	if (!m_shootArrow) return;
	m_shootArrow = false;
}

void Player::animationRanking(sf::Time /*dt*/) {}

void Player::animationControl() {}

void Player::drawDebug(sf::RenderTarget& target) {
	m_rectCollision.setPosition(
		m_position.x + m_solidArea.left,
		m_position.y + m_solidArea.top
	);
	m_rectCollision.setSize(
		sf::Vector2f(m_solidArea.width, m_solidArea.height)
	);
	m_rectCollision.setFillColor(sf::Color(0, 255, 0, 128));
	target.draw(m_rectCollision);
}
