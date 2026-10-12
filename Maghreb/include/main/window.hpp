#pragma once

#include <SFML/Graphics.hpp>
#include "main/shared_context.hpp"

class Window {
public:
	explicit Window(SharedContext& context);
	~Window();

	void update();

	sf::RenderWindow* getRenderWindow();
	const sf::Vector2u getWindowSize() const;
	sf::FloatRect getViewSpace();

	bool isFullscreen() const;
	bool isFocused() const;

	void close();

private:
	SharedContext&  m_context;
	sf::RenderWindow m_renderWindow;
	sf::Vector2u    m_windowSize;
	std::string     m_windowTitle;
	bool            m_isFullscreen;
	bool            m_isFocused;
};

