#pragma once

#include <SFML/Graphics.hpp>

class SpriteEffect {
public:
	// Обводка спрайта цветным контуром
	static void createStrokedSprite(sf::Sprite& outSprite,
		sf::Texture& outTexture,
		const sf::Sprite& source);

	// Красный оттенок (предмет нельзя использовать)
	static void createRedSprite(sf::Sprite& outSprite,
		sf::Texture& outTexture,
		const sf::Texture& source);
	
 private:

	// Проверка: пиксель непрозрачный?
	static bool isPixelSolid(const sf::Image& img, unsigned int x, unsigned int y);
};

