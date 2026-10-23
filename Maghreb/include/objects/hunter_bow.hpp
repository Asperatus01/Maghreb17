// entities/hunter_bow.hpp
#pragma once
#include "bow.hpp"

class HunterBow : public Bow {
private:
	//ANIMATION
	sf::Texture m_texture, m_strokeTexture, m_redTexture, m_primaryTexture;
	sf::Sprite m_primarySprite, m_redSprite, m_outputSprite;
	sf::Sprite m_firstFrame, m_secondFrame, m_lastFrame;
	sf::Sprite m_strokeSprite; // Подсветка предметов

	std::vector<sf::Sprite> m_spriteVect;
	

public:
	HunterBow(SharedContext& context);
	~HunterBow();
	
	// Update() пока не нужен, наследуем от Bow, но добавляем свою логику
	//void update(const sf::Time dT) override;

	// draw() не нужен, используем общий из Entity
};

