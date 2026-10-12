#include "objects/sprite_effect.hpp"
#include <iostream>

void SpriteEffect::createStrokedSprite(sf::Sprite& /*outSprite*/,
	sf::Texture& /*outTexture*/,
	const sf::Sprite& /*source*/) {
	std::cerr << "[Stub] SpriteEffect::createStrokedSprite\n";
}

void SpriteEffect::createRedSprite(sf::Sprite& /*outSprite*/,
	sf::Texture& /*outTexture*/,
	const sf::Texture& /*source*/) {
	std::cerr << "[Stub] SpriteEffect::createRedSprite\n";
}

bool SpriteEffect::isPixelSolid(const sf::Image& img, unsigned int x, unsigned int y) {
	return img.getPixel(x, y).a != 0;
}
