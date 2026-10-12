#pragma once

#include <string>

struct ItemStats {
	ItemStats()
		: m_minDamage(0), m_maxDamage(0)
		, m_minDefense(0), m_maxDefense(0)
		, m_defenseValue(0)
		, m_needStrength(0), m_needDexterity(0), m_needMagic(0)
		, m_durability(0), m_maxDurability(0)
		, m_neededCells(1)
		, m_attackSpeed(0), m_useCost(0)
		, m_price(0), m_group(0)
		, m_width(1), m_height(1)
		, m_restoresHealth(0), m_restoresMana(0)
		, m_recoveryTime(0)
		, m_increaseHealth(0), m_increaseStrength(0)
		, m_increaseDexterity(0), m_increaseMagic(0)
		, m_increaseVitality(0), m_increaseIllumRadius(0)
		, m_reducesDamage(0)
		, m_resistMagic(0), m_resistFire(0)
		, m_resistCold(0), m_resistLighting(0)
		, m_illuminationRadius(0) {}

	// Боевые
	int m_minDamage;
	int m_maxDamage;
	int m_minDefense;
	int m_maxDefense;
	int m_defenseValue;
	int m_attackSpeed;
	int m_useCost;

	// Требования
	int m_needStrength;
	int m_needDexterity;
	int m_needMagic;

	// Прочность
	int m_durability;
	int m_maxDurability;

	// Габариты в инвентаре
	int m_width;
	int m_height;
	int m_neededCells;

	// Экономика
	int m_price;
	int m_group;

	// Восстановление
	int m_restoresHealth;
	int m_restoresMana;
	int m_recoveryTime;

	// Бонусы к характеристикам
	int m_increaseHealth;
	int m_increaseStrength;
	int m_increaseDexterity;
	int m_increaseMagic;
	int m_increaseVitality;
	int m_increaseIllumRadius;

	// Сопротивления
	int m_reducesDamage;
	int m_resistMagic;
	int m_resistFire;
	int m_resistCold;
	int m_resistLighting;
	int m_illuminationRadius;

	std::wstring m_description;

	// Расчёт урона/защиты
	int getAttackValue() const;
	int getDefenseValue() const;
};

