#pragma once

#include "main/shared_context.hpp"
#include <SFML/Graphics.hpp>

class MouseHandler {
public:
	explicit MouseHandler(SharedContext& context);
	~MouseHandler();

	// Главная функция: обрабатывает клик и возвращает true, если клик был по чему-то игровому
	bool handleLeftClick();

	// Вспомогательная: просто получить мировые координаты (если понадобится в Game)
	sf::Vector2f getMouseWorldPos() const;

private:
	sf::Vector2i getPixelPos() const;
	sf::Vector2f pixelToWorld(const sf::Vector2i& mousePos) const;
	int checkClickOnObjects(const sf::Vector2f& worldPos); // Ищет объект под курсором и сразу вызывает onMouseClick

	SharedContext& m_context;
};
