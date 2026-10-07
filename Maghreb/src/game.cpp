#include <stdexcept>
#include <iostream>
#include "main/game.hpp"

// Фиксированный шаг времени: ровно 60 FPS
const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: m_window(sf::VideoMode(720, 720), "Maghreb")
{
	// Инициализируем камеру на весь экран/ставим в центр
	//m_view.reset(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
	m_view.setCenter(360.f, 360.f);
	m_window.setView(m_view);
}

void Game::run()
{
	setupGame();

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// Основной игровой цикл: пока окно открыто — крутимся
	while (m_window.isOpen())
	{
		processInput();

		// Накопление времени: если прошло больше timePerFrame,
		// обновляем логику несколько раз, чтобы игра не ускорялась/замедлялась
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}

		draw();
	}
}

void Game::setupGame()
{
	static const char* levels[] =
	{
		"assets/levels/Maghreb_level_01.tmx",
	};

	for (std::size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); ++i)
	{
		if (!m_tileManager.loadLevel(i, levels[i]))
		{
			throw std::runtime_error(std::string("Не удалось загрузить уровень: ") + levels[i]);
		}
	}

	// Активируем первый уровень
	m_tileManager.activate(0);

	// --- БЕРЁМ РАЗМЕР ТАЙЛА ПРЯМО ИЗ ЗАГРУЖЕННОГО УРОВНЯ ---
	const auto& level = m_tileManager.getCurrentLevel(); // нужен метод, который возвращает ссылку на активный Level
	const auto tileSize = level.getTileSize();           // это tmxlite::Map::getTileSize()
	const auto mapSize = level.getTileCount();           // размер карты в тайлах

	std::cout << "[DEBUG] Map size (tiles): " << mapSize.x << " x " << mapSize.y << "\n";
	std::cout << "[DEBUG] Tile size (px): " << tileSize.x << " x " << tileSize.y << "\n";
	std::cout << "[DEBUG] Total map size (px): "
		<< (mapSize.x * tileSize.x) << " x "
		<< (mapSize.y * tileSize.y) << "\n";

	// Центрируем камеру на карте
	const float centerX = static_cast<float>(mapSize.x) * tileSize.x * 0.5f;
	const float centerY = static_cast<float>(mapSize.y) * tileSize.y * 0.5f;

	m_view.setCenter(centerX, centerY);
	m_window.setView(m_view);
}


void Game::processInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		// Сюда позже можно добавить обработку клавиш для переключения уровней,
		// паузы, отладки и т.п.
	}
}

void Game::update(sf::Time const& deltaTime)
{
	const float speed = 400.f;

	// Временное управление камерой: стрелками двигаем view.
	// Это нужно, чтобы убедиться, что вся карта отрисовывается
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_view.move(-speed * deltaTime.asSeconds(), 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_view.move(speed * deltaTime.asSeconds(), 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_view.move(0.f, -speed * deltaTime.asSeconds());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_view.move(0.f, speed * deltaTime.asSeconds());

	m_window.setView(m_view);
}

void Game::draw()
{
	// Очищаем экран тёмно-серым
	m_window.clear(sf::Color(30, 30, 30));

	// Сначала рисуем саму карту (тайлы)
	m_tileManager.draw(m_window);

	// Отладочный прямоугольник — включать только когда нужно
    #ifdef DEBUG_BOUNDS
	{
		const auto& level = m_tileManager.getCurrentLevel();
		const auto tileSize = level.getTileSize();
		const auto mapSize = level.getTileCount();

		if (mapSize.x > 0 && mapSize.y > 0)
		{
			const float mapWidth = static_cast<float>(mapSize.x) * tileSize.x;
			const float mapHeight = static_cast<float>(mapSize.y) * tileSize.y;

			sf::RectangleShape debugRect(sf::Vector2f(mapWidth, mapHeight));
			debugRect.setFillColor(sf::Color(255, 0, 0, 60));
			debugRect.setOutlineThickness(2.f);
			debugRect.setOutlineColor(sf::Color::White);
			debugRect.setPosition(0.f, 0.f);

			m_window.draw(debugRect);
		}
	}
    #endif
	m_window.display();
}

