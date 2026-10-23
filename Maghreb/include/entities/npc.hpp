#pragma once

#include "entities/character.hpp"
#include "ai/dialogue_system.hpp"

class NPC : public Character {
public:
	explicit NPC(SharedContext& context);
	~NPC() override = default;

	DialogueSystem& getDialogue();

	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) const override;

	void interact() override;

private:
	DialogueSystem m_dialogue;
	std::wstring m_profession;
};

