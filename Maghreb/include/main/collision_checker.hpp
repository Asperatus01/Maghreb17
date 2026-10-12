#pragma once

#include "main/shared_context.hpp"
#include <vector>

// Forward declarations Ч не подключаем т€жЄлые заголовки
class Character;
class Entity;

class CollisionChecker {
public:
	explicit CollisionChecker(SharedContext& context);
	~CollisionChecker();

	// Ўаблонные методы Ч объ€влены, реализации в .inl (пустые заглушки)
	template<typename T>
	void findTile(T& entity, int row, int col);

	template<typename T>
	void checkTile(T& entity);

	void markTilesBlock(std::vector<Entity*> entityVect);
	void markTiles();

	Entity* checkEntity(Character& entity, std::vector<Entity*> target);
	bool checkPlayer(Character& entity);
	bool checkEdgeScreen(Character& entity);

private:
	SharedContext& m_context;
};

#include "collision_checker.inl"

