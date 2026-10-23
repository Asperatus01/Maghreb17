#pragma once

#include "entities/entity.hpp"
#include <string>

class BrokenColumn : public Entity {
public:
	explicit BrokenColumn(
		SharedContext& context,
		const wchar_t* name,
		const std::string& texturePath,
		sf::FloatRect defaultArea = { 0.f, 64.f, 32.f, 32.f }
	) : Entity(context)
	{
		m_name = name;

		m_solidAreaDefault = defaultArea;
		m_solidArea = defaultArea;

		sf::Texture& tex = AssetManager::getTexture(texturePath);
		m_staticSprite.setTexture(tex);

		setAnimationPointers(&m_staticSprite);

		m_type = EntityType::type_architecture;
		m_collision = true;
	}

	void setPosition(const sf::Vector2f& pos) override;
	void setPosition(float x, float y) override;

	virtual ~BrokenColumn() = default;

private:
	sf::Sprite m_staticSprite;
};
