#include "main/asset_setter.hpp"
#include "entities/entity.hpp"
#include <iostream>
#include <algorithm>

static constexpr float kTileSize = 32.f;

AssetSetter::AssetSetter(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] AssetSetter created\n";
}

AssetSetter::~AssetSetter() {
	std::cerr << "[Stub] AssetSetter destroyed\n";
}

Entity* AssetSetter::setEntity(Entity* entity, int tileX, int tileY) {
	if (!entity) return nullptr;

	float worldX = kTileSize * tileX;
	float worldY = kTileSize * tileY;

	// Ландшафт сдвигается на 16 пикселей по X
	if (entity->getType() == EntityType::type_landscape) {
		worldX -= 16.f;
	}

	entity->setPosition(sf::Vector2f(worldX, worldY));
	return entity;
}

void AssetSetter::deleteEntity(Entity* entity) {
	if (!entity) return;

	// TODO: реальная логика удаления с проверкой типа
	// Сейчас просто убираем nullptr из m_monsters
	m_monsters.erase(
		std::remove(m_monsters.begin(), m_monsters.end(), nullptr),
		m_monsters.end()
	);
}

void AssetSetter::setLandscape() {
	std::cerr << "[Stub] AssetSetter::setLandscape\n";
	// TODO: когда классы ландшафта (Tree, Column и т.д.) будут готовы
}

void AssetSetter::setObject() {
	std::cerr << "[Stub] AssetSetter::setObject\n";
	// TODO: когда классы предметов (Chest, Sarc, WeapStand и т.д.) будут готовы
}

void AssetSetter::setLight() {
	std::cerr << "[Stub] AssetSetter::setLight\n";
	// TODO: когда классы освещения (SkullFire, Candle и т.д.) будут готовы
}

void AssetSetter::setNPC() {
	std::cerr << "[Stub] AssetSetter::setNPC\n";
	// TODO: когда классы NPC (Healer и т.д.) будут готовы
}

void AssetSetter::setMonster() {
	std::cerr << "[Stub] AssetSetter::setMonster\n";
	// TODO: когда классы монстров (Skelbow, Falspear и т.д.) будут готовы
}

void AssetSetter::setInteractiveObjects() {
	std::cerr << "[Stub] AssetSetter::setInteractiveObjects\n";
	// TODO: когда классы интерактивных объектов (DrayTree и т.д.) будут готовы
}
