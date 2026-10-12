#pragma once

#include "shared_context.hpp"
#include <vector>

class Entity;
class Projectile;

class AssetSetter {
public:
	explicit AssetSetter(SharedContext& context);
	~AssetSetter();

	void deleteEntity(Entity* entity);

	void setLandscape();
	void setObject();
	void setLight();
	void setNPC();
	void setMonster();
	void setInteractiveObjects();

	std::vector<Entity*>     m_landscape;
	std::vector<Entity*>     m_monsters;
	std::vector<Entity*>     m_deadMonsters;
	std::vector<Entity*>     m_obj;
	std::vector<Entity*>     m_npc;
	std::vector<Entity*>     m_lightings;
	std::vector<Entity*>     m_iObjects;
	std::vector<Projectile*> m_projectiles;

private:
	Entity* setEntity(Entity* entity, int tileX, int tileY);
	SharedContext& m_context;
};

