#pragma once

#include <entities/entity.hpp>

class Tree : public Entity {
public:
	explicit Tree(
		SharedContext& context,
		const wchar_t* name,
		const std::string& texturePath,
		sf::FloatRect defaultArea = { 61.f, 118.f, 6.f, 6.f }
	) : Entity(context) {
		
		m_name = name;
			
		m_solidAreaDefault = defaultArea;
		m_solidArea = defaultArea;
		sf::Texture& tex = AssetManager::getTexture(texturePath);
		m_staticSprite.setTexture(tex);

		
		// 3. Подключаем спрайт к общей системе отрисовки.
		// Используем короткую перегрузку setAnimationPointers, чтобы не писать nullptr.
		setAnimationPointers(&m_staticSprite);

		m_type = EntityType::type_landscape;
		m_collision = true;
	}

	// ТОЛЬКО ОБЪЯВЛЕНИЕ: сигнатура + override. Тела функций здесь НЕТ!
	void setPosition(const sf::Vector2f& pos) override;
	void setPosition(float x, float y) override;
	virtual ~Tree() = default;

private:
	// Спрайт живёт здесь, внутри Tree.
	// Пока жив объект Tree, адрес &m_staticSprite валиден для указателя в Entity.
	sf::Sprite m_staticSprite;
};
