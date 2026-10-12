#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Заголовок лежит в include/main/shared_context.hpp,
// путь относительный от корня проекта (благодаря Additional Include Directories).
#include "main/shared_context.hpp"


/// Основной класс игры.
/// Наследуется от sf::NonCopyable: игра не должна копироваться.
class Game : sf::NonCopyable
{
public:
	explicit Game(SharedContext& context);
		
	~Game();

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

	// Только обязательные объекты на стеке
	SharedContext& m_context;    // контекст - держатель указателей на все объекты
	
	// Окно живёт в SharedContext::m_windPtr.
	// Здесь только невладеющий указатель для отрисовки.
	sf::RenderWindow* m_renderWindow;   // Окно и контекст рендера
	sf::View m_view;             // Камера (перемещаем её, чтобы двигать мир)
	 
	bool m_isPaused; // Отслеживание фокуса приложения
	
	// Невладеющие указатели на компоненты из контекста
	// Типы не нужны здесь: мы храним только указатели, а сами классы подключаем в .cpp
	class TileManager* m_tileManager = nullptr;  // Менеджер уровней (загрузка и отрисовка тайлов)

	/*Аналогично могут быть объявлены и другие указатели
	class CollisionChecker* m_checker = nullptr;
	class Player*           m_player = nullptr;*/

	///Блок переменных для подсчёта FPS
	sf::Clock m_fpsClock;
	int m_frameCount = 0;
	float m_currentFps = 0.0f;

	/// Блок переменных для вывода статистической информации
	sf::Font m_font;
	sf::Text m_fpsText;
	bool m_isFontLoaded = false;  // если шрифт загужен

};


