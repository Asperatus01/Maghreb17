#pragma once

#include "main/shared_context.hpp"
#include <SFML/Graphics.hpp>

struct EventDetails;
enum class StateType;

class MouseHandler {
public:
	explicit MouseHandler(SharedContext& context);
	~MouseHandler();

	void onCreate();

	sf::Vector2f getMouseWorldPos() const;
	sf::Vector2i getPixelPos() const;
	sf::Vector2i getWorldPos(const sf::Vector2i& mousePos) const;

	bool clickingPanels();

	bool m_leftMousePressed = false;
	bool m_rightMousePressed = false;

private:
	void actionLeftButton(EventDetails* details);
	void actionRightButton(EventDetails* details);

	bool getSolidTile();
	int  getSelectedEntity();
	int  scanningInteractiveTile();
	int  scanningMonsters();
	int  scanningMonstersLeft();
	int  scanningMonstersRight();
	int  scanningNpc();
	int  scanningObjects();

	SharedContext& m_context;
	sf::Vector2f   m_mouseWorldPos;
	bool           m_stayAway = false;
};

