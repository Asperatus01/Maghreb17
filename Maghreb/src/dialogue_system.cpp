#include "ai/dialogue_system.hpp"
#include <iostream>

DialogueSystem::DialogueSystem() {
	std::cerr << "[Stub] DialogueSystem created\n";
}

void DialogueSystem::loadDialogueSet(int /*setIndex*/) {
	std::cerr << "[Stub] DialogueSystem::loadDialogueSet\n";
}

void DialogueSystem::startDialogue(SharedContext& /*context*/) {
	m_active = true;
	m_currentIndex = 0;
	std::cerr << "[Stub] Dialogue started\n";
}

void DialogueSystem::nextLine() {
	std::cerr << "[Stub] DialogueSystem::nextLine\n";
}

void DialogueSystem::endDialogue() {
	m_active = false;
	std::cerr << "[Stub] Dialogue ended\n";
}

bool DialogueSystem::isActive() const { return m_active; }
int DialogueSystem::getCurrentSet() const { return m_currentSet; }
int DialogueSystem::getCurrentIndex() const { return m_currentIndex; }

void DialogueSystem::draw(sf::RenderTarget& /*target*/, const sf::FloatRect& /*viewSpace*/) {
	// «аглушка: отрисовка диалогового окна
}
