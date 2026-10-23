#pragma once

#include <SFML/Graphics.hpp>

class Utils {
public:
	static constexpr float PI = 3.14159265358979323846f;

	static float radiansToDegrees(float radians) {
		return radians * (180.0f / PI);
	}

	static float degreesToRadians(float degrees) {
		return degrees * (PI / 180.f);
	}

	// Просто фабричная функция: текстура → спрайт.
	// Никаких зависимостей от контекста, никаких менеджеров внутри.
	static sf::Sprite createCursorSprite(const sf::Texture& texture) {
		return sf::Sprite(texture);
	}
};

