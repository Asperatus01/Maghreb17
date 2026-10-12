#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "main/shared_context.hpp"
#include "objects/item_stats.hpp"

enum class EntityType {
	type_unknown,
	type_hands,
	type_sword,
	type_axe,
	type_shield,
	type_helmet,
	type_bow,
	type_arrow,
	type_firebolt,
	type_gold,
	type_potion,
	type_scroll,
	type_lightArmor,
	type_middleArmor,
	type_heavyArmor,
	type_staff,
	type_mace,
	type_amulet,
	type_barrel,
	type_obstacle,// тип препятствие
	type_landscape,
	type_ring,
	type_book,
	type_cash,
	type_chargedbolt,
	type_healing,
	type_lighting,
	type_architecture,
	type_magic,
	type_monster,
	type_player,
	type_npc,
	type_item,
	type_interactive
};

class Entity {
public:
	explicit Entity(SharedContext& context);
	virtual ~Entity() = default;

	// --- Имя / Уникальный ID (новое) ---
	const std::wstring& getName() const { return m_name; }
	void setName(const std::wstring& name) { m_name = name; }
	void setName(const std::string& nameUtf8) {
		// Если CSV в UTF-8, а нужно wstring — можно добавить простую конвертацию, 
		// но пока оставим только wstring-версию для простоты.
		m_name = std::wstring(nameUtf8.begin(), nameUtf8.end());
	}

	sf::Vector2f getPosition() const { return m_position; }
	void setPosition(const sf::Vector2f& pos) {	m_position = pos; }

	sf::FloatRect getSolidArea() const { return m_solidArea;	}
	void setSolidArea(const sf::FloatRect& area) {	m_solidArea = area;	}

	sf::FloatRect getWorldCoordSolid() const;
	sf::Vector2f getCenter() const;

	EntityType getType() const { return m_type;	}
	void setType(EntityType type) {	m_type = type; }

	bool getCollision() const {	return m_collision;	}
	void setCollision(bool collision) {	m_collision = collision; }

	SharedContext& getContext() const {	return m_context; }

	// ВАЖНО: теперь возвращаем указатель, а не ссылку, так как статы может и не быть
	ItemStats* getItemStats() {
		return m_itemStats.has_value() ? &m_itemStats.value() : nullptr;
	}

	const ItemStats* getItemStats() const {
		return m_itemStats.has_value() ? &m_itemStats.value() : nullptr;
	}

	void setInInventory(bool value) { m_inInventory = value; }
	bool isInInventory() const { return m_inInventory; }

	void setDressed(bool value) { m_dressed = value; }
	bool isDressed() const { return m_dressed; }

	void setTwoHandedWeapon(bool value) { m_two_handed_weapon = value; }
	bool isTwoHandedWeapon() const { return m_two_handed_weapon; }

	void setShowItem(bool value) { m_showItem = value; }
	bool isShowItem() const { return m_showItem; }

	void setOpened(bool value) { m_opened = value; }
	bool isOpened() const { return m_opened; }

	virtual void update(sf::Time dt);
	virtual void draw(sf::RenderTarget& target);
	virtual void interact();
	virtual void use();

protected:
	SharedContext& m_context;      // указатель вместо ссылки
	sf::Vector2f m_position;
	sf::FloatRect m_solidArea;
	sf::FloatRect m_solidAreaDefault;
	EntityType m_type = EntityType::type_unknown;

	bool m_inInventory = false;
	bool m_dressed = false;
	bool m_showItem = false;
	bool m_two_handed_weapon = false;
	bool m_opened = false;
	bool m_collision = true;                // <-- добавлено

private:
	std::optional<ItemStats>  m_itemStats;

	// Имя = уникальный идентификатор (из CSV)
	std::wstring m_name;
};
