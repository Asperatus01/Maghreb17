#include "main/mouse_handler.hpp"
#include "main/asset_setter.hpp"
#include "main/shared_context.hpp"
#include "main/window.hpp"
#include "entities/entity.hpp"
#include <iostream>

MouseHandler::MouseHandler(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[INFO] MouseHandler created\n";
}

MouseHandler::~MouseHandler() {
	std::cerr << "[INFO] MouseHandler destroyed\n";
}

// Просто берём позицию мыши относительно экрана (работает всегда)
sf::Vector2i MouseHandler::getPixelPos() const {
	return sf::Mouse::getPosition(*m_context.m_renderWindowPtr);
}

// Переводим экранные пиксели в координаты игрового мира через камеру
sf::Vector2f MouseHandler::pixelToWorld(const sf::Vector2i& mousePos) const {
	if (!m_context.m_windowPtr) {
		std::cerr << "[WARNING] MouseHandler::pixelToWorld called without valid window. Using raw pixel coords.\n";
		return { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
	}

	const sf::View& view = *m_context.m_view;
	// mapPixelToCoords уже учитывает вид, который установлен на окне
	return m_context.m_windowPtr->getRenderWindow()->mapPixelToCoords(mousePos, view);

}

// Удобная обертка: сразу получить мировые координаты курсора
sf::Vector2f MouseHandler::getMouseWorldPos() const {
	return pixelToWorld(getPixelPos());
}


// Сканируем объекты (ларцы) и вызываем реакцию у того, по которому кликнули
/// Проверяет, по какому объекту был клик, и вызывает его реакцию.
/// Возвращает индекс объекта в m_obj, если клик попал в объект, или -1, если нет.
/// Важно: порядок объектов должен совпадать с порядком отрисовки (снизу → вверх).

/// worldPos - мировые координаты щелчка мыши
int MouseHandler::checkClickOnObjects(const sf::Vector2f& worldPos) {
	if (!m_context.m_assetSetterPtr) {
		return -1;
	}

	const auto& objects = m_context.m_assetSetterPtr->m_obj;

	for (size_t i = 0; i < objects.size(); ++i) {
		const auto& obj = objects[i];
		if (!obj) continue; // Защита от nullptr в unique_ptr
		// Проверяем попадание в хитбокс
		if (obj->contains(worldPos)) {
			obj->onMouseClick(worldPos); // Вызываем реакцию объекта
			return static_cast<int>(i);
		}
	}

	return -1;
}

//// Главная функция обработки левого клика
//bool MouseHandler::handleLeftClick() {
//	auto worldPos = getMouseWorldPos();
//	int objIndex = checkClickOnObjects(worldPos);
//
//	if (objIndex != -1) {
//		// Клик был по объекту: onMouseClick уже сработал внутри scanningObjects
//		return true; // Клик «съеден» игровым объектом
//	}
//
//	// Клик мимо объектов
//	return false;
//}

bool MouseHandler::handleLeftClick() {
	// Получем экранные координаты щелчка мыши
	sf::Vector2i const mouseScreenPos = getPixelPos();
	// 1. Получаем мировые координаты курсора (это критично!)
	//sf::Vector2f mouseWorldPos = pixelToWorld(mouseScreenPos);

	// 2. ПОЛУЧАЕМ МИР ЧЕРЕЗ ТВОЮ ФУНКЦИЮ (это единственный правильный путь)
	sf::Vector2f mouseWorldPos = getMouseWorldPos();

	std::cerr << "[MOUSE] Left click at screen: " << mouseScreenPos.x << ", " << mouseScreenPos.y << "\n";
	std::cerr << "[MOUSE] World pos: " << mouseWorldPos.x << ", " << mouseWorldPos.y << "\n";

	// 2. Проверяем именно тот вектор, где лежат ларцы (скорее всего m_obj)
	const auto& objects = m_context.m_assetSetterPtr->m_obj;
	std::cerr << "[MOUSE] Checking " << objects.size() << " objects in m_obj\n";

	int hitCount = 0;
	for (size_t i = 0; i < objects.size(); ++i) {
		const auto& obj = objects[i];
		if (!obj) continue;

		bool isHit = obj->contains(mouseWorldPos);
		if (isHit) {
			std::cerr << "[MOUSE] HIT #" << i << " type=" << static_cast<int>(obj->getType())
				<< " name=" << obj->getName().c_str() << "\n";
			obj->onMouseClick(mouseWorldPos);
			++hitCount;
			// Если хочешь обрабатывать только первый попавшийся — раскомментируй return true;
			// return true; 
		}
	}

	if (hitCount == 0) {
		std::cerr << "[MOUSE] No object hit in m_obj.\n";
		return false;
	}
	return true;
}

