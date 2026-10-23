#pragma once
#include <entities/entity.hpp>

class Bow : public Entity
{
public:
	Bow(SharedContext& context);
	virtual               ~Bow();
	void                  update(const sf::Time dT);
	void                  draw(sf::RenderTarget& target);
};
