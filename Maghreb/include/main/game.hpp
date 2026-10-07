#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Заголовок лежит в include/loader/tile_manager.hpp,
// путь относительный от корня проекта (благодаря Additional Include Directories).
#include "loader/tile_manager.hpp"

/// Основной класс игры.
/// Наследуется от sf::NonCopyable: игра не должна копироваться.
class Game : sf::NonCopyable
{
public:
	Game();

	/// Главный цикл игры: ввод, обновление, отрисовка.
	void run();

private:
	void setupGame();
	void processInput();
	void update(sf::Time const& deltaTime);
	void draw();

private:
	/// Фиксированный шаг времени для игрового цикла (60 FPS).
	static const sf::Time timePerFrame;

	sf::RenderWindow m_window;   // Окно и контекст рендера
	sf::View m_view;             // Камера (перемещаем её, чтобы двигать мир)
	TileManager m_tileManager;  // Менеджер уровней (загрузка и отрисовка тайлов)
};

#endif // GAME_HPP

