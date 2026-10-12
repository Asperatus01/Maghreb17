#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class SharedContext;

class DialogueSystem {
public:
	DialogueSystem();

	void loadDialogueSet(int setIndex);
	void startDialogue(SharedContext& context);
	void nextLine();
	void endDialogue();

	bool isActive() const;
	int getCurrentSet() const;
	int getCurrentIndex() const;

	void draw(sf::RenderTarget& target, const sf::FloatRect& viewSpace);

private:
	struct DialogueLine {
		std::wstring m_speaker;
		std::wstring m_text;
	};

	std::vector<std::vector<DialogueLine>> m_dialogueSets;
	int m_currentSet = 0;
	int m_currentIndex = 0;
	bool m_active = false;
	sf::Text m_text;
};

