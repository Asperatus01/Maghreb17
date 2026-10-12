#pragma once

class Entity;

struct Equipment {
	Equipment()
		: m_weapon(nullptr)
		, m_magic(nullptr)
		, m_shield(nullptr)
		, m_helmet(nullptr)
		, m_armor(nullptr)
		, m_amulet(nullptr)
		, m_leftRing(nullptr)
		, m_rightRing(nullptr) {}

	Entity* m_weapon;
	Entity* m_magic;
	Entity* m_shield;
	Entity* m_helmet;
	Entity* m_armor;
	Entity* m_amulet;
	Entity* m_leftRing;
	Entity* m_rightRing;

	bool isTwoHanded() const;
	void clear();
};

