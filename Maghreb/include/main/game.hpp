#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "main/shared_context.hpp"
#include "entities/entity.hpp"

enum class CursorState {
	Hand,       // Палец (по умолчанию)
	ItemDrag,   // Перетаскиваемый предмет
	Normal      // Системный курсор Windows (когда мышь вне окна)
};

class Game : sf::NonCopyable
{
public:
	explicit Game(SharedContext& context);
	~Game();

	void run();

private:
	void setupGame();
	bool createCursor(const std::string& path);
	void processInput(const sf::Event& event);
	void handleContinuousInput();
	void update(sf::Time const& deltaTime);
	void draw();

	// --- Состояние и данные курсора (только внутри класса!) ---
	sf::Image m_cursorImage;
	sf::Cursor m_cursor;
	bool m_isCursorSet = false;
	// Флаг «мышь в окне» всё ещё нужен для игровой логики (клики и т.п.)
	bool m_isCursorInWindow = false;

	// --- Фиксированный шаг времени ---
	static const sf::Time timePerFrame;

	// --- Контекст и окно ---
	SharedContext& m_context;
	sf::RenderWindow* m_renderWindow;
	sf::View m_view;
	bool m_isPaused;

	// --- Компоненты (невладеющие указатели) ---
	class TileManager* m_tileManager = nullptr;

	// --- FPS и UI ---
	sf::Clock m_fpsClock;
	int m_frameCount = 0;
	float m_currentFps = 0.0f;

	sf::Font m_font;
	sf::Text m_fpsText;
	bool m_isFontLoaded = false;
};
