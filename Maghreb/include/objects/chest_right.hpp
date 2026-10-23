#pragma once

#include "entities/entity.hpp"
#include <iostream>

class ChestRight : public Entity {
private:
	std::vector<sf::Sprite> m_spriteVect; // все 3 кадра для правого ларца
	size_t m_currentFrameIndex = 0;       // индекс текущего кадра (0,1,2)
	bool m_isOpened = false;             // уже открыт?

	 // Таймер храним внутри класса, а не static (иначе все ларцы будут анимироваться синхронно!)
	float m_openTimer = 0.0f;
	const float m_frameDuration = 0.15f; // 150 мс на кадр

	bool m_lootGiven = false;

public:
	explicit ChestRight(
		SharedContext& context,
		const wchar_t* name,
		sf::FloatRect defaultArea = { 32.f, 73.f, 23.f, 13.f }
	) : Entity(context)
	{
		auto stats = getItemStats();
		m_name = name;
		m_type = EntityType::type_obstacle;
		if (stats != nullptr) {
			//stats->m_description = L"Ларец (правый)"; 
		}
		m_collision = true;

		m_solidAreaDefault = defaultArea;
		m_solidArea = defaultArea;

		// 1. Нарезаем все 6 кадров
		std::vector<sf::Sprite> allFrames;
		Entity::getAnimation("res/objects/storage/chest1.png", 6, 1, allFrames);

		// 2. Берём только последние 3 кадра
		if (allFrames.size() >= 6) {
			m_spriteVect.assign(allFrames.begin() + 3, allFrames.end()); // кадры 3,4,5
		}
		// Стань так:
		if (!m_spriteVect.empty()) {
			// Мы НЕ создаем копию. Мы просто запоминаем индекс.
			// Но так как Entity::draw ждет указатель на sf::Sprite, нам нужно хитрость.

			// ХИТРОСТЬ: Используем первый кадр вектора как "текущий".
			// Нам нужно, чтобы setAnimationPointers указывал на элемент вектора!
			setAnimationPointers(&m_spriteVect[0]);
		}
	}
	bool contains(const sf::Vector2f& pos) const override;
	void onMouseClick(const sf::Vector2f& pos) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setPosition(float x, float y) override;

	// Обновляем кадр каждый кадр (dt)
	/// ВАЖНО: та же сигнатура, что и в Entity
	void update(sf::Time dt) override;

	virtual ~ChestRight() = default;
};

