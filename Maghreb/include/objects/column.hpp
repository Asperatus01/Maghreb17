#pragma once

#include "entities/entity.hpp"
#include <string>

class Column : public Entity {
public:
	explicit Column(
		SharedContext& context,
		const wchar_t* name,
		const std::string& texturePath,
		sf::FloatRect defaultArea = { 0.f, 64.f, 32.f, 32.f }
	) : Entity(context)
	{
		m_name = name;

		m_solidAreaDefault = defaultArea;
		m_solidArea = defaultArea;

		// Загрузка текстуры через AssetManager (как в Tree)
		sf::Texture& tex = AssetManager::getTexture(texturePath);
		m_staticSprite.setTexture(tex);

		// Подключаем спрайт к общей системе отрисовки
		setAnimationPointers(&m_staticSprite);

		m_type = EntityType::type_architecture;
		m_collision = true;
	}

	// ТОЛЬКО ОБЪЯВЛЕНИЕ (по шаблону Tree). Без тела!
	void setPosition(const sf::Vector2f& pos) override;
	void setPosition(float x, float y) override;

	virtual ~Column() = default;

private:
	// Спрайт живёт здесь, внутри Column.
	// Пока жив объект Column, адрес &m_staticSprite валиден.
	sf::Sprite m_staticSprite;
};

