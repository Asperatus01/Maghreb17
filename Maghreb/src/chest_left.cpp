#include "objects/chest_left.hpp"
#include <iostream>

//bool ChestLeft::contains(const sf::Vector2f& pos) const {
//	// Получаем плотную зону уже в мировых координатах
//	const auto hitbox = getWorldCoordSolid();
//
//	// Защита от некорректного прямоугольника (если спрайт не задан или зона нулевая)
//	if (hitbox.width <= 0.f || hitbox.height <= 0.f) {
//		return false;
//	}
//	return hitbox.contains(pos);
//}

bool ChestLeft::contains(const sf::Vector2f& pos) const {
	const auto hitbox = getWorldCoordSolid();
	// Защита от некорректного прямоугольника (если спрайт не задан или зона нулевая)
	if (hitbox.width <= 0.f || hitbox.height <= 0.f) {
		return false;
	}
	std::cerr << "[HITBOX] SolidAreaDefault = {"
		<< m_solidAreaDefault.left << ", "
		<< m_solidAreaDefault.top << ", "
		<< m_solidAreaDefault.width << "x"
		<< m_solidAreaDefault.height << "}\n";

	std::cerr << "[HITBOX] m_solidArea (actual) = {"
		<< hitbox.left << ", "
		<< hitbox.top << ", "
		<< hitbox.width << "x"
		<< hitbox.height << "}\n";

	std::cerr << "[HITBOX] Mouse world pos = ("
		<< pos.x << ", " << pos.y << ")\n";

	bool result = hitbox.contains(pos);
	std::cerr << "[HITBOX] Contains? " << (result ? "YES" : "NO") << "\n\n";
	return result;
}



void ChestLeft::onMouseClick(const sf::Vector2f& /*pos*/) {
	if (m_isOpened) {
		return; // Уже открыт — ничего не делаем
	}

	m_isOpened = true;          // помечаем, что начали открывать
	m_currentFrameIndex = 0;    // начинаем с первого кадра
	setAnimationPointers(&m_spriteVect[m_currentFrameIndex]);; // сразу рисуем первый кадр

	std::cerr << "[EVENT] ChestLeft clicked: opening started.\n";
	// Тут можно добавить проигрывание звука (через твой SoundManager)
}




void ChestLeft::setPosition(const sf::Vector2f& pos) {
	for (auto& sprite : m_spriteVect) {
		sprite.setPosition(pos);
	}
	Entity::setPosition(pos);
}

void ChestLeft::setPosition(float x, float y) {
	sf::Vector2f pos(x, y);
	for (auto& sprite : m_spriteVect) {
		sprite.setPosition(pos);
	}
	Entity::setPosition(x, y);
}

void ChestLeft::update(sf::Time dt) {
	// Конвертируем в float (в секундах) — это стандарт для игровой логики
	const float frameDt = dt.asSeconds();

	if (!m_isOpened) {
		return;
	}

	m_openTimer += frameDt;

	if (m_openTimer >= m_frameDuration) {
		m_openTimer -= m_frameDuration;

		++m_currentFrameIndex;
		if (m_currentFrameIndex >= m_spriteVect.size()) {
			m_currentFrameIndex = m_spriteVect.size() - 1;
		}

		// Обновляем указатель, который использует Entity::draw
		setAnimationPointers(&m_spriteVect[m_currentFrameIndex]);

		// Если дошли до последнего кадра — ларец открыт
		if (m_currentFrameIndex == m_spriteVect.size() - 1) {
			// Флаг уже стоит true, но можно добавить выдачу лута ровно один раз
			if (!m_lootGiven && m_currentFrameIndex == static_cast<int>(m_spriteVect.size()) - 1) {
				m_lootGiven = true;
				std::cerr << "[EVENT] ChestLeft fully opened, loot given.\n";
				// Тут логика выдачи предмета: m_context.m_aSetterPtr->m_obj.push_back(...)
			}
		}
	}
}


