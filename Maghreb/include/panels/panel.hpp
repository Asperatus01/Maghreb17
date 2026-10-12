#pragma once

#include "main/shared_context.hpp"
#include <SFML/Graphics.hpp>

class Panel {
public:
	explicit Panel(SharedContext& context);
	virtual ~Panel() = default;

	virtual void onCreate() {}
	virtual void loadResources() {}
	virtual void createPanelButton() {}
	virtual void displayItems() {}
	virtual void update() {}
	virtual void draw() {}

protected:
	SharedContext& m_context;
	sf::Vector2i m_screenCoord{ 0, 0 };
	sf::FloatRect m_panelRect{ 0, 0, 0, 0 };
	sf::Vector2f m_worldCoord{ 0, 0 };
	sf::Vector2f m_panelPosition{ 0, 0 };

	bool inBounds(const sf::IntRect& rect, const sf::Vector2i& point) const;
};

