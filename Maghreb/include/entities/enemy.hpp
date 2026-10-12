#pragma once

#include "character.hpp"
#include "ai/ai_controller.hpp"
#include "ai/loot_system.hpp"

class Enemy : public Character {
public:
	explicit Enemy(SharedContext& context);
	~Enemy() override = default;

	AiController& getAi();
	LootSystem& getLoot();

	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) override;

	// Поведение
	void setAction();

private:
	AiController m_ai;
	LootSystem m_loot;

	bool m_resurrection = false;
	bool m_reborn = false;
	bool m_offBalance = false;
	int m_offBalanceCounter = 0;
	int m_startTileRow = 0;
	int m_startTileCol = 0;
};

