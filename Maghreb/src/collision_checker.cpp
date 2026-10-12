#include "main/collision_checker.hpp"
#include <iostream>

CollisionChecker::CollisionChecker(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] CollisionChecker created\n";
}

CollisionChecker::~CollisionChecker() {
	std::cerr << "[Stub] CollisionChecker destroyed\n";
}

void CollisionChecker::markTilesBlock(std::vector<Entity*> /*entityVect*/) {
	// Заглушка: не обращаемся к TileManager и AssetSetter
}

void CollisionChecker::markTiles() {
	// Заглушка: пропускаем вызовы к m_context.m_aSetterPtr
}

Entity* CollisionChecker::checkEntity(Character& /*entity*/, std::vector<Entity*> /*target*/) {
	// Заглушка: возвращаем nullptr — коллизий нет
	return nullptr;
}

bool CollisionChecker::checkPlayer(Character& /*entity*/) {
	// Заглушка: контакта с игроком нет
	return false;
}

bool CollisionChecker::checkEdgeScreen(Character& /*entity*/) {
	// Заглушка: считаем, что сущность всегда в пределах экрана
	return true;
}
