#include "entities/npc.hpp"
#include <iostream>

NPC::NPC(SharedContext& context)
	: Character(context)
{
	std::cerr << "[Stub] NPC created\n";
	m_type = EntityType::type_npc;
	m_collision = false; // NPC обычно не блокируют движение
}

DialogueSystem& NPC::getDialogue() { return m_dialogue; }

void NPC::update(sf::Time /*dt*/) {
	// Заглушка: NPC стоят на месте или ходят по маршрутам
}

void NPC::draw(sf::RenderTarget& /*target*/) {
	// Заглушка
}

void NPC::interact() {
	m_dialogue.startDialogue(m_context);
}
