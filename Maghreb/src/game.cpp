#include <stdexcept>
#include <iostream>
#include "main/game.hpp"

// Полные заголовки — здесь, в .cpp, это безопасно.
// Теперь мы можем спокойно включать всё, что нужно для new/delete.
#include "main/window.hpp"
#include "loader/tile_manager.hpp"
#include "main/event_manager.hpp"          // Раскомментировал, так как используется
#include "main/collision_checker.hpp"
#include "ai/path_finder.hpp"
#include "entities/player.hpp"
#include "main/asset_setter.hpp"
#include "panels/control_panel.hpp"
#include "main/mouse_handler.hpp"
#include "sound/music_player.hpp"
#include "sound/sound_player.hpp"         // Обязательно для new SoundPlayer
#include "panels/inventory_panel.hpp"    // Обязательно для new InventoryPanel
#include "panels/inventory.hpp"           // Обязательно для new Inventory
#include "main/animation_holder.hpp"   


// Фиксированный шаг времени: ровно 60 FPS
const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game(SharedContext& context)
	: m_context(context)
{
	// --- ИНИЦИАЛИЗАЦИЯ (через new вместо make_unique) ---

	// 1. Создаём Window в куче
	m_context.m_windowPtr = new Window(m_context);
	

	// 2. Получаем невладеющий указатель на внутренний RenderWindow
	// Важно: убедись, что метод getRenderWindow() возвращает sf::RenderWindow*
	m_context.m_renderWindowPtr = m_context.m_windowPtr->getRenderWindow();

	// 3. Сохраняем для удобного доступа в Game
	m_renderWindow = m_context.m_renderWindowPtr;

	if (!m_renderWindow) {
		throw std::runtime_error("Не удалось получить RenderWindow из Window");
	}
	else {
		m_renderWindow->setVerticalSyncEnabled(true);
	}

	// Инициализация view: размер = размер окна, центр пока по центру окна
	const auto size = m_renderWindow->getSize();
	m_view.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y)));

	if (m_renderWindow) {
		m_renderWindow->setView(m_view);
	}
	// Передаём вид в контекст, видом владеет Game
	m_context.m_view = &m_view;

	// 5. Создаём остальные базовые объекты в куче через new
	// Порядок важен: сначала независимые, потом зависимые.
	m_context.m_tileManagerPtr = new TileManager();
	//m_context.m_checkerPtr = new CollisionChecker(context);
	m_context.m_pathFinderPtr = new PathFinder(context);
	m_context.m_eventManagerPtr = new EventManager(context);
	m_context.m_mouseHandlerPtr = new MouseHandler(context);
	m_context.m_musicPlayerPtr = new MusicPlayer();
	m_context.m_soundPlayerPtr = new SoundPlayer();

	// Объекты, которым может понадобиться инвентарь/панели (создаем после них)
	m_context.m_inventoryPtr = new Inventory(context);
	//m_context.m_iPanelPtr = new InventoryPanel(context); // Панель зависит от инвентаря
	m_context.m_controlPanelPtr = new ControlPanel(context);
	m_context.m_assetSetterPtr = new AssetSetter(context);
	m_context.m_animHolderPtr = new AnimationHolder();
	m_context.m_itemAnimHolderPtr = new AnimationHolder(); 

	// «Верхние» объекты
	m_context.m_playerPtr = new Player(context);

	// 6. Запоминаем невладеющие указатели для быстрого доступа внутри Game
	m_tileManager = m_context.m_tileManagerPtr;
	

	// Загрузка шрифта
	m_isFontLoaded = m_font.loadFromFile("res/fonts/arial.ttf");

	if (!m_isFontLoaded) {
		std::cerr << "[WARNING] Шрифт не загружен — FPS не будет отображаться на экране.\n";
	}
	else {
		m_fpsText.setFont(m_font);
		m_fpsText.setString("FPS: 0");
		m_fpsText.setFillColor(sf::Color::White);
		m_fpsText.setCharacterSize(24);
		m_fpsText.setPosition(10.f, 10.f);
	}
}

// --- ДЕСТРУКТОР (ОБЯЗАТЕЛЬНО: ручное удаление всех объектов) ---
Game::~Game() {
	// ВАЖНО: Порядок удаления должен быть обратным порядку создания,
	// особенно если объекты имеют зависимости друг от друга.
	delete m_context.m_itemAnimHolderPtr;
	delete m_context.m_animHolderPtr;
	delete m_context.m_playerPtr;             // Player может использовать другие системы
	delete m_context.m_assetSetterPtr;       // AssetSetter может держать ресурсы
	delete m_context.m_controlPanelPtr;      // ControlPanel
	//delete m_context.m_iPanelPtr;            // InventoryPanel (зависит от Inventory, удаляем ДО него)
	delete m_context.m_inventoryPtr;          // Inventory
	delete m_context.m_soundPlayerPtr;       // SoundPlayer
	delete m_context.m_musicPlayerPtr;       // MusicPlayer
	delete m_context.m_mouseHandlerPtr;
	delete m_context.m_eventManagerPtr;
	delete m_context.m_pathFinderPtr;
	//delete m_context.m_checkerPtr;
	delete m_context.m_tileManagerPtr;

	//delete m_context.m_viewPtr;              // sf::View
	delete m_context.m_windowPtr;              // Window (удалит внутри себя RenderWindow)
}

// Установка игры
void Game::setupGame() {
	static const char* levels[] = {
		"res/levels/Maghreb_level_01.tmx",
	};

	for (std::size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); ++i) {
		if (!m_tileManager->loadLevel(i, levels[i])) {
			throw std::runtime_error(std::string("Не удалось загрузить уровень: ") + levels[i]);
		}
	}

	m_tileManager->activate(0);

	const auto& level = m_tileManager->getCurrentLevel();
	const auto tileSize = level.getTileSize();
	const auto mapSize = level.getTileCount();

	std::cout << "[DEBUG] Map size (tiles): " << mapSize.x << " x " << mapSize.y << "\n";
	std::cout << "[DEBUG] Tile size (px): " << tileSize.x << " x " << tileSize.y << "\n";

	const float mapWidth = static_cast<float>(mapSize.x) * tileSize.x;
	const float mapHeight = static_cast<float>(mapSize.y) * tileSize.y;

	// Центрируем камеру на карте
	m_view.setCenter(mapWidth * 0.5f, mapHeight * 0.5f);
	if (m_renderWindow) {
		m_renderWindow->setView(m_view);
	}

	// Создаём игровой курсор
	createCursor("res/cursor/cursorfinger.png");
	
	// Запускаем методы класса AssetSetter
	m_context.m_assetSetterPtr->setLandscape();
	m_context.m_assetSetterPtr->setMonster();
	m_context.m_assetSetterPtr->setNPC();
	m_context.m_assetSetterPtr->setObject();
	m_context.m_assetSetterPtr->setLight();
	m_context.m_assetSetterPtr->setInteractiveObjects();
}

bool Game::createCursor(const std::string& path) {
	// Инициализация курсора-указателя
	if (!m_cursorImage.loadFromFile(path)) {
		std::cerr << "Error: Could not load cursor image!" << std::endl;
		// Если картинки нет — просто не включаем кастомный курсор, будет обычная стрелка
		return m_isCursorSet = false;
	}

	const std::uint8_t* pixels = m_cursorImage.getPixelsPtr();
	if (pixels) {
		// Hotspot: подбери под свой палец. (0,0) — клик в левом верхнем углу картинки.
		// Часто для пальца ставят что-то вроде (12, 24) или (16, 32) — поэкспериментируй.
		m_cursor.loadFromPixels(pixels, m_cursorImage.getSize(), sf::Vector2u(0, 0));
		return m_isCursorSet = true;
	}
}


void Game::run() {
	setupGame();
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (m_renderWindow && m_renderWindow->isOpen()) {
		sf::Event event;

		// Читаем ВСЕ события, которые накопились за кадр
		while (m_renderWindow->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_renderWindow->close();
			}
			else {
				// Передаем каждое событие в обработчик
				processInput(event);
			}
		}

		// Сюда можно добавить обработку состояния (WASD), если не хочешь делать её в update()
		handleContinuousInput(); 

		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
		draw();
	}
}

void Game::processInput(const sf::Event& event) {
	// 1. Обработка мгновенных действий (нажатия клавиш)
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::F1) {
			m_context.toggleDebugMode();
			return; // Сразу выходим: для этого события больше ничего не нужно
		}

		if (event.key.code == sf::Keyboard::Space) {
			// Логика прыжка или блока щитом (одно нажатие)
			if (m_context.m_playerPtr) {
				// Блок щитом
				//m_context.m_playerPtr->blockWithShield();
			}
		}
		// ... другие мгновенные действия (атака, открытие инвентаря и т.д.)
	}

	// 2. Обработка мыши (клики, скролл)
	// Используем MouseButtonReleased — это защищает от «дребезга» и множественных срабатываний
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			// Проверяем, кликнули ли мы по какому-то игровому объекту (ларцу и т.п.)
			bool handled = m_context.m_mouseHandlerPtr->handleLeftClick();

			if (!handled) {
				// Клик в пустоту → запускаем движение персонажа
				sf::Vector2f targetPos = m_context.m_mouseHandlerPtr->getMouseWorldPos();

				if (m_context.m_playerPtr) {
					//m_context.m_playerPtr->setTargetPosition(targetPos);

					// Лог для отладки (можно убрать в релизе)
					std::cerr << "[DEBUG] Left button released, no object hit. Moving player to ("
						<< targetPos.x << ", " << targetPos.y << ")\n";
				}
			}
			else {
				// Клик обработан объектом (например, ларец открылся)
				// Движение НЕ запускаем.
				std::cerr << "[DEBUG] Left button released, click handled by game object (no movement)\n";
			}
		}

		// Правая кнопка (если нужна)
		if (event.mouseButton.button == sf::Mouse::Right) {
			// Например, отмена действия или контекстное меню
			// if (m_context.m_playerPtr) m_context.m_playerPtr->cancelAction();
		}
	}

	if (event.type == sf::Event::MouseWheelMoved) {
		// Скролл инвентаря или камеры
		// int delta = static_cast<int>(event.mouseWheelMoved.delta);
		// if (m_context.m_inventoryPtr) m_context.m_inventoryPtr->scroll(delta);
	}
}


void Game::handleContinuousInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//m_player->move(0, -speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//m_player->move(0, speed);
	}
	// ... остальные зажатые клавиши
}



void Game::update(sf::Time const& deltaTime) {
	const float speed = 400.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_view.move(-speed * deltaTime.asSeconds(), 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_view.move(speed * deltaTime.asSeconds(), 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_view.move(0.f, -speed * deltaTime.asSeconds());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_view.move(0.f, speed * deltaTime.asSeconds());

	// Ограничение камеры границами карты
	const float mapWidth = 60.f * 32.f;   // 1920 px
	const float mapHeight = 60.f * 32.f; // 1920 px

	const float halfViewW = m_view.getSize().x / 2.f;
	const float halfViewH = m_view.getSize().y / 2.f;

	float centerX = m_view.getCenter().x;
	float centerY = m_view.getCenter().y;

	// Левая граница
	if (centerX - halfViewW < 0.f)
		centerX = halfViewW;
	// Правая граница
	if (centerX + halfViewW > mapWidth)
		centerX = mapWidth - halfViewW;

	// Верхняя граница
	if (centerY - halfViewH < 0.f)
		centerY = halfViewH;
	// Нижняя граница
	if (centerY + halfViewH > mapHeight)
		centerY = mapHeight - halfViewH;

	m_view.setCenter(centerX, centerY);

	for (auto& obj : m_context.m_assetSetterPtr->m_obj) {
		if (obj) {
			obj->update(deltaTime); // Сюда приходит sf::Time, и у ChestLeft вызывается правильный override
		}
	}

}


void Game::draw() {
	if (!m_renderWindow) return;

	m_renderWindow->clear(sf::Color(30, 30, 30));

	// --- 1. Рисуем мир (карта, игрок и т.д.) с камерой ---
	// Важно: сначала setView, потом draw мира
	m_renderWindow->setView(m_view);

	if (m_tileManager) {
		m_tileManager->draw(*m_renderWindow);
	}
	// !!! ВСТАВКА: Отрисовка деревьев (ландшафта) и объектов!!!
	if (m_context.m_assetSetterPtr) {
		const auto& landscape = m_context.m_assetSetterPtr->m_landscape;

		for (const auto& treePtr : landscape) {
			treePtr->draw(*m_renderWindow);
		}
		const auto& obj = m_context.m_assetSetterPtr->m_obj;
		for (const auto& objPtr : obj)
			objPtr->draw(*m_renderWindow);
	}
	// !!! КОНЕЦ ВСТАВКИ !!!
	if (m_context.m_playerPtr) {
		m_context.m_playerPtr->draw(*m_renderWindow);
	}
	// Сюда позже вернёшь визуализацию коллизий, если нужно

	// --- 2. Возвращаем стандартный view для UI (FPS, панель и т.п.) ---
	m_renderWindow->setView(m_renderWindow->getDefaultView());

	// --- Блок расчёта и отрисовки FPS ---
	m_frameCount++;
	const float elapsed = m_fpsClock.getElapsedTime().asSeconds();
	if (elapsed >= 1.0f) {
		m_currentFps = static_cast<float>(m_frameCount) / elapsed;
		m_frameCount = 0;
		m_fpsClock.restart();
	}

	if (m_isFontLoaded) {
		// Теперь эти координаты — экранные (пиксели окна), а не мировые
		m_fpsText.setPosition(10.f, 10.f);
		m_fpsText.setString("FPS: " + std::to_string(static_cast<int>(m_currentFps)));
		m_renderWindow->draw(m_fpsText);
	}

	// Установка курсора
	if (m_isCursorSet) {
		m_renderWindow->setMouseCursor(m_cursor);
	}

	m_renderWindow->display();
}
