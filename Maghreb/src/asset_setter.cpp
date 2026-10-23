#include "main/asset_setter.hpp"
#include "entities/entity.hpp"
#include <iostream>
#include <algorithm>
#include <objects/landscape.hpp>
#include <objects/column.hpp>
#include <objects/broken_column.hpp>
#include <objects/chest_left.hpp>
#include <objects/chest_right.hpp>

static constexpr float kTileSize = 32.f;

AssetSetter::AssetSetter(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] AssetSetter created\n";
}


Entity* AssetSetter::setEntity(std::unique_ptr<Entity>& entity, int tileX, int tileY) {
	if (!entity) return nullptr;

	float worldX = kTileSize * tileX;
	float worldY = kTileSize * tileY;

	// Сдвиг для ландшафта
	if (entity->getType() == EntityType::type_landscape) {
		worldX -= 16.f;
	}
	entity->setPosition(sf::Vector2f(worldX, worldY));
	return entity.get(); // Возвращаем сырой указатель для удобства, но владеем через unique_ptr
}


void AssetSetter::deleteEntity(Entity* entity) {
	if (!entity) return;

	// TODO: реальная логика удаления с проверкой типа
	// Сейчас просто убираем nullptr из m_monsters
	/*m_monsters.erase(
		std::remove(m_monsters.begin(), m_monsters.end(), nullptr),
		m_monsters.end()
	);*/
}

void AssetSetter::setLandscape() {
	std::cerr << "[INFO] Setting landscape for map: " << m_context.m_currentMap << "\n";

	if (m_context.m_currentMap == 0) {
		// --- tree01 (Obj_Tree01) ---
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree01_1", "res/objects/trees/tree01.png"));
		setEntity(m_landscape.back(), 20, 45);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree01_2", "res/objects/trees/tree01.png"));
		setEntity(m_landscape.back(), 21, 44);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree01_3", "res/objects/trees/tree01.png"));
		setEntity(m_landscape.back(), 22, 43);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree01_4", "res/objects/trees/tree01.png"));
		setEntity(m_landscape.back(), 24, 41);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree01_5", "res/objects/trees/tree01.png"));
		setEntity(m_landscape.back(), 20, 40);
		// --- tree03 (Obj_Tree03) ---
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree03_1", "res/objects/trees/tree03.png"));
		setEntity(m_landscape.back(), 19, 41);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree03_2", "res/objects/trees/tree03.png"));
		setEntity(m_landscape.back(), 17, 43);
		// --- tree02 (Obj_Tree02) ---
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_1", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 27, 36);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_2", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 29, 35);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_3", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 30, 34);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_4", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 35, 33);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_5", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 39, 34);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_6", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 28, 35);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree02_7", "res/objects/trees/tree02.png"));
		setEntity(m_landscape.back(), 32, 35);
		// --- tree09 (Obj_Tree09) ---
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree09_1", "res/objects/trees/tree09.png"));
		setEntity(m_landscape.back(), 32, 35);
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree09_2", "res/objects/trees/tree09.png"));
		setEntity(m_landscape.back(), 33, 34);
		// --- tree05 (Obj_Tree05) ---
		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree05_1", "res/objects/trees/tree05.png"));
		setEntity(m_landscape.back(), 40, 27);

		m_landscape.push_back(std::make_unique<Tree>(m_context, L"tree05_2", "res/objects/trees/tree05.png"));
		setEntity(m_landscape.back(), 38, 29);

		m_landscape.push_back(std::make_unique<Column>(m_context, L"column01", "res/objects/column/column.png"));
		setEntity(m_landscape.back(), 46, 31);

		m_landscape.push_back(std::make_unique<BrokenColumn>(m_context, L"broken_column01", "res/objects/column/broken_column.png"));
		setEntity(m_landscape.back(), 47, 27);
	}
}


void AssetSetter::setObject() {
	if (m_context.m_currentMap == 0) {
		m_obj.push_back(std::make_unique<ChestLeft>(m_context, L"Ларец_левый"));
		setEntity(m_obj.back(), 35, 37);

		m_obj.push_back(std::make_unique<ChestRight>(m_context, L"Ларец_правый"));
		setEntity(m_obj.back(), 45, 42);
	}
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
