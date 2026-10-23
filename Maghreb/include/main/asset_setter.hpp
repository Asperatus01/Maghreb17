#pragma once

#include "shared_context.hpp"
#include <vector>
#include <memory> // <-- Обязательно для unique_ptr

class Entity;
class Projectile;

class AssetSetter {
public:
	explicit AssetSetter(SharedContext& context);
	~AssetSetter() = default; // Деструктор теперь можно оставить пустым!

	void deleteEntity(Entity* entity); // Логика чуть изменится, см. ниже

	void setLandscape();
	void setObject();
	void setLight();
	void setNPC();
	void setMonster();
	void setInteractiveObjects();

	
	std::vector<std::unique_ptr<Entity>>     m_landscape;
    std::vector<std::unique_ptr<Entity>>     m_obj;
	//std::vector<std::unique_ptr<Entity>>     m_monsters;
	//std::vector<std::unique_ptr<Entity>>     m_deadMonsters;
	//std::vector<std::unique_ptr<Entity>>     m_npc;
	//std::vector<std::unique_ptr<Entity>>     m_lightings;
	//std::vector<std::unique_ptr<Entity>>     m_iObjects;*/

	// Для Projectile тоже лучше сделать unique_ptr, если они тоже создаются через new
	//std::vector<std::unique_ptr<Projectile>> m_projectiles;

private:
	// Возвращаем сырой указатель только для логики позиционирования, 
	// но храним внутри unique_ptr.
	Entity* setEntity(std::unique_ptr<Entity>& entity, int tileX, int tileY);
	SharedContext& m_context;
};
