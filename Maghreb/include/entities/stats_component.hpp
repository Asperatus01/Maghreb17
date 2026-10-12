#pragma once

struct StatsComponent {
	StatsComponent()
		: m_level(1), m_experience(0), m_nextLevelExp(100)
		, m_life(10), m_maxLife(10)
		, m_mana(30), m_maxMana(30)
		, m_strength(20), m_defaultStrength(20)
		, m_dexterity(20), m_defaultDexterity(20)
		, m_magic(20), m_defaultMagic(20)
		, m_vitality(10), m_defaultVitality(10)
		, m_armorClass(1), m_accuracy(100)
		, m_speed(100.f), m_defaultSpeed(100.f)
		, m_attack(0), m_defense(0)
		, m_pointsToDistribute(5)
		, m_detDistance(16)
		, m_talkDistance(1)
		, m_attackDistance(1)
		, m_damage(0) {}

	int m_level;
	int m_experience;
	int m_nextLevelExp;

	int m_life;
	int m_maxLife;
	int m_mana;
	int m_maxMana;

	int m_strength;
	int m_defaultStrength;
	int m_dexterity;
	int m_defaultDexterity;
	int m_magic;
	int m_defaultMagic;
	int m_vitality;
	int m_defaultVitality;

	int m_armorClass;
	int m_accuracy;

	float m_speed;
	float m_defaultSpeed;

	int m_attack;
	int m_defense;
	int m_attackDistance;
	int m_detDistance;
	int m_talkDistance;
	int m_pointsToDistribute;
	int m_damage;

	void resetToDefaults();
	void applyLevelUp();
};

