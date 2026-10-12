#include "panels/inventory.hpp"
#include "entities/player.hpp"
#include <iostream>
#include <cassert>

static constexpr int kScreenWidth = 800;
static constexpr int kScreenHeight = 600;
static constexpr int kTileSize = 32;

Inventory::Inventory(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] Inventory created\n";
}

Inventory::~Inventory() {
	for (auto& row : m_cellIP)
		for (auto* cell : row)
			delete cell;
	for (auto* cell : m_cellCP)
		delete cell;
	delete m_weaponsCell;
	delete m_shieldCell;
	delete m_helmetCell;
	delete m_armorCell;
	delete m_amuletCell;
	delete m_leftRingCell;
	delete m_rightRingCell;
}

// =========================================================================
// onCreate
// =========================================================================

void Inventory::onCreate() {
	m_screenCoord.x = kScreenWidth / 2;
	m_screenCoord.y = kScreenHeight - 496;

	// TODO: m_font = AssetManager::GetFont("Resources/Font/arial.ttf");
	m_information.setFont(m_font);
	m_information.setStyle(sf::Text::Italic);
	m_information.setCharacterSize(14);
	m_information.setFillColor(sf::Color::White);

	createCellsIP();
	createCellsCP();
	setItems();
}

// =========================================================================
// Создание ячеек
// =========================================================================

void Inventory::createCellsCP() {
	// TODO: позиция от ControlPanel
	// sf::Vector2i positionCP = m_context.m_cPanelPtr->getScreenCoord();
	sf::Vector2i positionCP{ 0, 0 };
	for (int col = 0; col < m_maxColCP; ++col) {
		sf::Vector2i position{ positionCP.x + col * 29 + 205, positionCP.y + 22 };
		m_cellCP.push_back(new Cell(position));
	}
}

void Inventory::createCellsIP() {
	m_helmetCell = new Cell(m_screenCoord.x + 135, m_screenCoord.y + 5);
	m_helmetCell->m_cellRect = sf::IntRect(m_screenCoord.x + 135, m_screenCoord.y + 5, 52, 52);

	m_weaponsCell = new Cell(m_screenCoord.x + 19, m_screenCoord.y + 76);
	m_weaponsCell->m_cellRect = sf::IntRect(m_screenCoord.x + 19, m_screenCoord.y + 76, 55, 83);

	m_armorCell = new Cell(m_screenCoord.x + 135, m_screenCoord.y + 77);
	m_armorCell->m_cellRect = sf::IntRect(m_screenCoord.x + 135, m_screenCoord.y + 77, 55, 83);

	m_shieldCell = new Cell(m_screenCoord.x + 250, m_screenCoord.y + 77);
	m_shieldCell->m_cellRect = sf::IntRect(m_screenCoord.x + 250, m_screenCoord.y + 77, 55, 83);

	m_amuletCell = new Cell(m_screenCoord.x + 206, m_screenCoord.y + 34);
	m_amuletCell->m_cellRect = sf::IntRect(m_screenCoord.x + 206, m_screenCoord.y + 34, 26, 26);

	m_leftRingCell = new Cell(m_screenCoord.x + 48, m_screenCoord.y + 178);
	m_leftRingCell->m_cellRect = sf::IntRect(m_screenCoord.x + 48, m_screenCoord.y + 178, 26, 26);

	m_rightRingCell = new Cell(m_screenCoord.x + 248, m_screenCoord.y + 178);
	m_rightRingCell->m_cellRect = sf::IntRect(m_screenCoord.x + 248, m_screenCoord.y + 178, 26, 26);

	for (int row = 0; row < m_maxRowIP; ++row) {
		std::vector<Cell*> tempVect;
		for (int col = 0; col < m_maxColIP; ++col) {
			sf::Vector2i position{ m_screenCoord.x + col * 29 + 18, m_screenCoord.y + row * 29 + 222 };
			tempVect.push_back(new Cell(position));
		}
		m_cellIP.push_back(tempVect);
	}
}

// =========================================================================
// Первоначальное заполнение
// =========================================================================

void Inventory::setItems() {
	m_inventoryPanelMap.clear();
	m_controlPanelMap.clear();

	// TODO: создание предметов когда классы объектов будут готовы
	// setInventory(new Obj_Buckler(m_context));
	// setInventory(new Obj_ShortBow(m_context));
	// setInventory(new Obj_Axe(m_context));
	// setInventory(new Obj_Cap(m_context));
	// setInventory(new Obj_Cape(m_context));
	// setInventory(new Obj_HealthPotion(m_context));
	// setInventory(new Obj_ManaPotion(m_context));
	// setInventory(new Obj_Club(m_context));
	std::cerr << "[Stub] Inventory::setItems\n";
}

// =========================================================================
// Поиск по типу
// =========================================================================

bool Inventory::findWeapon(EntityType type) const {
	return type == EntityType::type_bow
		|| type == EntityType::type_axe
		|| type == EntityType::type_sword
		|| type == EntityType::type_mace
		|| type == EntityType::type_staff;
}

bool Inventory::findCloseCombatWeapons(EntityType type) const {
	return type == EntityType::type_axe
		|| type == EntityType::type_sword
		|| type == EntityType::type_mace
		|| type == EntityType::type_staff
		|| type == EntityType::type_shield;
}

bool Inventory::findArmor(EntityType type) const {
	return type == EntityType::type_lightArmor
		|| type == EntityType::type_middleArmor
		|| type == EntityType::type_heavyArmor;
}

// =========================================================================
// Размещение предметов
// =========================================================================

bool Inventory::setInventory(Entity* entity) {
	if (!entity) return false;

	bool wasPlaced = setCurrentItems(entity);
	if (wasPlaced) return true;

	const auto* stats = entity->getItemStats();
	if (!stats) {
		// У предмета нет статистики — либо это не предмет, либо ошибка инициализации
		return false;
	}

	int neededCells = stats->m_neededCells;


	// 1-клеточные → командная панель
	if (neededCells == 1 && m_controlPanelMap.size() < 8) {
		for (int i = 0; i < 8; ++i) {
			if (m_cellCP[i]->m_free) {
				m_controlPanelMap.emplace(entity, m_cellCP[i]->m_cellRect);
				m_cellCP[i]->m_free = false;
				return true;
			}
		}
	}

	// 1-клеточные → нижняя строка панели инвентаря
	if (neededCells == 1 && m_controlPanelMap.size() >= 8) {
		for (int i = 0; i < m_maxColIP; ++i) {
			if (m_cellIP[3][i]->m_free) {
				m_inventoryPanelMap.emplace(entity, m_cellIP[3][i]->m_cellRect);
				m_cellIP[3][i]->m_free = false;
				return true;
			}
		}
	}

	// 3-клеточные (мечи, кинжалы)
	if (neededCells == 3) {
		for (int i = 0; i < m_maxColIP; ++i) {
			if (m_cellIP[0][i]->m_free) {
				sf::IntRect rect{ m_cellIP[0][i]->m_cellRect.left, m_cellIP[0][i]->m_cellRect.top, 28, 29 * 3 };
				m_inventoryPanelMap.emplace(entity, rect);
				m_cellIP[0][i]->m_free = false;
				m_cellIP[1][i]->m_free = false;
				m_cellIP[2][i]->m_free = false;
				return true;
			}
		}
	}

	// 4-клеточные (шлемы, книги, щиты)
	if (neededCells == 4) {
		for (int i = 0; i < m_maxColIP - 1; ++i) {
			if (m_cellIP[0][i]->m_free && m_cellIP[0][i + 1]->m_free) {
				sf::IntRect rect{ m_cellIP[0][i]->m_cellRect.left, m_cellIP[0][i]->m_cellRect.top, 28 * 2, 29 * 2 };
				m_inventoryPanelMap.emplace(entity, rect);
				m_cellIP[0][i]->m_free = false;
				m_cellIP[0][i + 1]->m_free = false;
				m_cellIP[1][i]->m_free = false;
				m_cellIP[1][i + 1]->m_free = false;
				return true;
			}
			if (m_cellIP[2][i]->m_free && m_cellIP[2][i + 1]->m_free
				&& m_cellIP[3][i]->m_free && m_cellIP[3][i + 1]->m_free) {
				sf::IntRect rect{ m_cellIP[2][i]->m_cellRect.left, m_cellIP[2][i]->m_cellRect.top, 28 * 2, 29 * 2 };
				m_inventoryPanelMap.emplace(entity, rect);
				m_cellIP[2][i]->m_free = false;
				m_cellIP[2][i + 1]->m_free = false;
				m_cellIP[3][i]->m_free = false;
				m_cellIP[3][i + 1]->m_free = false;
				return true;
			}
		}
	}

	// 6-клеточные (двуручное оружие, луки)
	if (neededCells == 6) {
		for (int i = 0; i < m_maxColIP - 1; ++i) {
			if (m_cellIP[0][i]->m_free && m_cellIP[0][i + 1]->m_free) {
				sf::IntRect rect{ m_cellIP[0][i]->m_cellRect.left, m_cellIP[0][i]->m_cellRect.top, 28 * 2, 29 * 3 };
				m_inventoryPanelMap.emplace(entity, rect);
				m_cellIP[0][i]->m_free = false;
				m_cellIP[0][i + 1]->m_free = false;
				m_cellIP[1][i]->m_free = false;
				m_cellIP[1][i + 1]->m_free = false;
				m_cellIP[2][i]->m_free = false;
				m_cellIP[2][i + 1]->m_free = false;
				return true;
			}
		}
	}

	return false;
}

// =========================================================================
// Размещение в слотах снаряжения
// =========================================================================

bool Inventory::setCurrentItems(Entity* entity) {
	// TODO: первый элемент — всегда Cash
	// if (m_inventoryPanelMap.empty())
	//     m_inventoryPanelMap.emplace(new Obj_Cash(m_context), m_cellIP[3][0]->m_cellRect);
	// m_cellIP[3][0]->m_free = false;

	return setCurrentEquipment(entity, m_helmetCell)
		|| setCurrentEquipment(entity, m_weaponsCell)
		|| setCurrentEquipment(entity, m_armorCell)
		|| setCurrentEquipment(entity, m_shieldCell)
		|| setCurrentEquipment(entity, m_amuletCell)
		|| setCurrentEquipment(entity, m_leftRingCell)
		|| setCurrentEquipment(entity, m_rightRingCell);
}

bool Inventory::setCurrentEquipment(Entity* entity, Cell* cell) {
	if (!entity || !cell) return false;
	if (!checkCorrespondenceCells(entity, cell) || !cell->m_free || !canPlaceItem(entity))
		return false;

	entity->setInInventory(true);
	m_inventoryPanelMap.emplace(entity, cell->m_cellRect);
	cell->m_free = false;

	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return true;                            // защита от nullptr
	//Player* player = playerPtr.get();                       // получаем сырой указатель

	Player* player = m_context.m_playerPtr;
	if (!player) return true; // Защита от nullptr
	if (!player) return true;

	auto& eq = player->getEquipment();

	if (m_helmetCell == cell) {
		eq.m_helmet = entity;
		eq.m_helmet->setDressed(true);
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_helmet);
	}
	else if (m_weaponsCell == cell) {
		setTwoHandedWeapons(entity, true);
		eq.m_weapon = entity;
		eq.m_weapon->setDressed(true);
		// TODO: m_context.m_animHolderPtr->changeAnimationType();
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_weapon);
	}
	else if (m_armorCell == cell) {
		eq.m_armor = entity;
		eq.m_armor->setDressed(true);
		// TODO: m_context.m_animHolderPtr->changeAnimationType();
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_armor);
	}
	else if (m_shieldCell == cell) {
		eq.m_shield = entity;
		eq.m_shield->setDressed(true);
		// TODO: m_context.m_animHolderPtr->changeAnimationType();
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_shield);
	}
	else if (m_amuletCell == cell) {
		eq.m_amulet = entity;
		eq.m_amulet->setDressed(true);
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_amulet);
	}
	else if (m_leftRingCell == cell) {
		eq.m_leftRing = entity;
		eq.m_leftRing->setDressed(true);
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_leftRing);
	}
	else if (m_rightRingCell == cell) {
		eq.m_rightRing = entity;
		eq.m_rightRing->setDressed(true);
		// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_rightRing);
	}

	return true;
}

bool Inventory::checkCorrespondenceCells(Entity* entity, Cell* cell) const {
	if (!entity || !cell) return false;

	auto type = entity->getType();
	if (type == EntityType::type_helmet && m_helmetCell == cell) return true;
	if (findWeapon(type) && m_weaponsCell == cell) return true;
	if (findArmor(type) && m_armorCell == cell) return true;
	if (type == EntityType::type_shield && m_shieldCell == cell) return true;
	if (type == EntityType::type_amulet && m_amuletCell == cell) return true;
	if (type == EntityType::type_ring && (m_leftRingCell == cell || m_rightRingCell == cell)) return true;
	return false;
}

bool Inventory::canPlaceItem(Entity* entity) const {
	if (!entity) return false;

	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return true;                            // защита от nullptr
	//Player* player = playerPtr.get();                       // получаем сырой указатель

	Player* player = m_context.m_playerPtr;
	if (!player) return true; // Защита от nullptr


	const auto* stats = entity->getItemStats();
	if (!stats) return false; // защита: если статистики нет — предмет нельзя экипировать

	const auto& playerStats = player->getStats();

	if (playerStats.m_strength < stats->m_needStrength) return false;
	if (playerStats.m_dexterity < stats->m_needDexterity) return false;
	if (playerStats.m_magic < stats->m_needMagic) return false;
	return true;
}

// =========================================================================
// Двуручное оружие
// =========================================================================

void Inventory::setTwoHandedWeapons(Entity* entity, bool add) {
	if (!entity) return;

	bool isTwoHanded = entity->isTwoHandedWeapon();
	if (!isTwoHanded) return;

	if (add) {
		// TODO: создать клон через EntityGenerator
		// m_inventoryPanelMap.emplace(
		//     m_context.m_eGeneratorPtr->getObject(entity->getName()),
		//     m_shieldCell->m_cellRect
		// );
		m_shieldCell->m_free = false;
	}
	else {
		// Удаляем клон из ячейки щита
		Entity* toRemove = nullptr;
		for (const auto&[key, val] : m_inventoryPanelMap) {
			if (val == m_shieldCell->m_cellRect) {
				toRemove = key;
				break;
			}
		}
		if (toRemove) m_inventoryPanelMap.erase(toRemove);
		m_shieldCell->m_free = true;
	}
}

// =========================================================================
// Ручное размещение
// =========================================================================

bool Inventory::manageCurrentItems(Entity* entity) {
	return addToCell(entity, m_helmetCell)
		|| addToCell(entity, m_weaponsCell)
		|| addToCell(entity, m_armorCell)
		|| addToCell(entity, m_shieldCell)
		|| addToCell(entity, m_amuletCell)
		|| addToCell(entity, m_leftRingCell)
		|| addToCell(entity, m_rightRingCell)
		|| addToSharedCell(entity);
}

bool Inventory::addToCell(Entity* entity, Cell* cell) {
	if (!entity || !cell) return false;

	// TODO: sf::RenderWindow* window = m_context.m_windPtr->getRenderWindow();
	sf::Vector2i mousePos{ 0, 0 };
	bool leftMousePressed = false;

	// TODO: проверка нажатия мыши и m_cycleCounter

	if (!cell->m_cellRect.contains(mousePos.x, mousePos.y))
		return false;
	if (!checkCorrespondenceCells(entity, cell) || !cell->m_free || !canPlaceItem(entity))
		return false;

	m_largeCursorInstalled = false;
	entity->setInInventory(true);
	m_inventoryPanelMap.emplace(entity, cell->m_cellRect);
	cell->m_free = false;
	rearrangeItem(entity);

	// TODO: m_context.m_cPanelPtr->soundAddingItem(entity);

	
	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return true;                            // защита от nullptr
	//Player* player = playerPtr.get();           // получаем сырой указатель для чтения
	
	Player* player = m_context.m_playerPtr;
	if (!player) return true; // Защита от nullptr
	if (player) {
		auto& eq = player->getEquipment();

		if (m_helmetCell == cell) {
			eq.m_helmet = entity;
			eq.m_helmet->setDressed(true);
			// TODO: recalculateData
		}
		else if (m_weaponsCell == cell) {
			setTwoHandedWeapons(entity, true);
			eq.m_weapon = entity;
			eq.m_weapon->setDressed(true);
			// TODO: changeAnimationType + recalculateData
		}
		else if (m_armorCell == cell) {
			eq.m_armor = entity;
			eq.m_armor->setDressed(true);
			// TODO: changeAnimationType + recalculateData
		}
		else if (m_shieldCell == cell) {
			eq.m_shield = entity;
			eq.m_shield->setDressed(true);
			// TODO: changeAnimationType + recalculateData
		}
		else if (m_amuletCell == cell) {
			eq.m_amulet = entity;
			eq.m_amulet->setDressed(true);
		}
		else if (m_leftRingCell == cell) {
			eq.m_leftRing = entity;
			eq.m_leftRing->setDressed(true);
		}
		else if (m_rightRingCell == cell) {
			eq.m_rightRing = entity;
			eq.m_rightRing->setDressed(true);
		}
	}

	return true;
}

bool Inventory::releaseItem(Entity* entity, Cell* cell) {
	if (!entity || !cell) return false;
	m_largeCursorInstalled = false;
	entity->setInInventory(true);
	m_inventoryPanelMap.emplace(entity, cell->m_cellRect);
	cell->m_free = false;
	rearrangeItem(entity);
	// TODO: m_context.m_cPanelPtr->soundAddingItem(entity);
	return true;
}

bool Inventory::addToSharedCell(Entity* entity) {
	if (!entity) return false;

	// TODO: получение позиции мыши и проверка нажатия
	sf::Vector2i mousePos{ 0, 0 };
	bool leftMousePressed = false;

	int neededCells = entity->getItemStats()->m_neededCells;
	int maxRow = 0, maxCol = 0;

	if (neededCells == 1) { maxRow = 4; maxCol = 10; }
	else if (neededCells == 3) { maxRow = 2; maxCol = 10; }
	else if (neededCells == 4) { maxRow = 3; maxCol = 9; }
	else if (neededCells == 6) { maxRow = 2; maxCol = 9; }
	else return false;

	for (int row = 0; row < maxRow; ++row) {
		for (int col = 0; col < maxCol; ++col) {
			if (!m_cellIP[row][col]->m_cellRect.contains(mousePos.x, mousePos.y))
				continue;
			if (!leftMousePressed || !m_cellIP[row][col]->m_free)
				continue;

			if (neededCells == 1) {
				return releaseItem(entity, m_cellIP[row][col]);
			}
			if (neededCells == 3) {
				if (m_cellIP[row][col]->m_free && m_cellIP[row + 1][col]->m_free && m_cellIP[row + 2][col]->m_free) {
					m_cellIP[row][col]->m_free = false;
					m_cellIP[row + 1][col]->m_free = false;
					m_cellIP[row + 2][col]->m_free = false;
					return releaseItem(entity, m_cellIP[row][col]);
				}
			}
			if (neededCells == 4) {
				if (m_cellIP[row][col]->m_free && m_cellIP[row][col + 1]->m_free
					&& m_cellIP[row + 1][col]->m_free && m_cellIP[row + 1][col + 1]->m_free) {
					m_cellIP[row][col]->m_free = false;
					m_cellIP[row][col + 1]->m_free = false;
					m_cellIP[row + 1][col]->m_free = false;
					m_cellIP[row + 1][col + 1]->m_free = false;
					return releaseItem(entity, m_cellIP[row][col]);
				}
			}
			if (neededCells == 6) {
				if (m_cellIP[row][col]->m_free && m_cellIP[row][col + 1]->m_free
					&& m_cellIP[row + 1][col]->m_free && m_cellIP[row + 1][col + 1]->m_free
					&& m_cellIP[row + 2][col]->m_free && m_cellIP[row + 2][col + 1]->m_free) {
					m_cellIP[row][col]->m_free = false;
					m_cellIP[row][col + 1]->m_free = false;
					m_cellIP[row + 1][col]->m_free = false;
					m_cellIP[row + 1][col + 1]->m_free = false;
					m_cellIP[row + 2][col]->m_free = false;
					m_cellIP[row + 2][col + 1]->m_free = false;
					return releaseItem(entity, m_cellIP[row][col]);
				}
			}
		}
	}
	return false;
}

bool Inventory::autoAdditionToSharedCells(Entity* entity) {
	if (!entity) return false;

	int maxRow = 2;
	int maxCol = 9;

	for (int row = 0; row < maxRow; ++row) {
		for (int col = 0; col < maxCol; ++col) {
			if (m_cellIP[row][col]->m_free && m_cellIP[row][col + 1]->m_free
				&& m_cellIP[row + 1][col]->m_free && m_cellIP[row + 1][col + 1]->m_free
				&& m_cellIP[row + 2][col]->m_free && m_cellIP[row + 2][col + 1]->m_free) {
				m_cellIP[row][col]->m_free = false;
				m_cellIP[row][col + 1]->m_free = false;
				m_cellIP[row + 1][col]->m_free = false;
				m_cellIP[row + 1][col + 1]->m_free = false;
				m_cellIP[row + 2][col]->m_free = false;
				m_cellIP[row + 2][col + 1]->m_free = false;
				return releaseItem(entity, m_cellIP[row][col]);
			}
		}
	}
	return false;
}

void Inventory::rearrangeItem(Entity* entity) {
	if (!entity || !entity->isTwoHandedWeapon()) return;
	if (m_shieldCell->m_free) return;

	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return;                            // защита от nullptr
	//Player* player = playerPtr.get();                       // получаем сырой указатель
	
	Player* player = m_context.m_playerPtr;
	if (!player) return; // Защита от nullptr

	auto& eq = player->getEquipment();
	if (!eq.m_shield) return;

	eq.m_shield->setInInventory(false);
	eq.m_shield->setDressed(false);
	// TODO: m_context.m_animHolderPtr->recalculateData(eq.m_shield);

	Entity* oldShield = eq.m_shield;
	// TODO: m_context.m_cPanelPtr->setGlobalEntity(oldShield);
	eq.m_shield = nullptr;

	// Удаляем щит из карты
	m_inventoryPanelMap.erase(oldShield);

	bool wasPlaced = autoAdditionToSharedCells(oldShield);
	if (!wasPlaced) {
		// Выбрасываем в мир
		// TODO: m_context.m_aSetterPtr->m_obj.push_back(oldShield);
		// oldShield->setPosition(sf::Vector2f(player->getPosition().x, player->getPosition().y - 100));
		// oldShield->m_showItem = true;
		// TODO: m_context.m_cPanelPtr->setGlobalEntity(nullptr);
	}
}

// =========================================================================
// Информация и использование
// =========================================================================

void Inventory::getInfoIP() {
	// TODO: sf::RenderWindow* window = m_context.m_windPtr->getRenderWindow();
	// sf::Vector2f worldTextCoord = window->mapPixelToCoords(
	//     sf::Vector2i(kScreenWidth / 2, kScreenHeight - 64));
	// sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(*window);
	//
	// for (auto& [entity, rect] : m_inventoryPanelMap) {
	//     if (rect.contains(mouseScreenPos.x, mouseScreenPos.y)) {
	//         std::wstring desc = entity->getItemStats().m_description;
	//         std::wstringstream ss(desc);
	//         std::wstring line;
	//         while (std::getline(ss, line)) {
	//             m_information.setString(line);
	//             ... отрисовка
	//         }
	//     }
	// }
}

void Inventory::useItemIP() {
	// TODO: получение позиции мыши, проверка правого клика
	// Собираем элементы для удаления, потом удаляем (безопасно)
	std::vector<Entity*> toRemove;

	sf::Vector2i mouseScreenPos{ 0, 0 }; // TODO: sf::Mouse::getPosition(*window)
	bool rightMousePressed = false;    // TODO: sf::Mouse::isButtonPressed(sf::Mouse::Right)

	if (!rightMousePressed) return;

	for (auto&[entity, rect] : m_inventoryPanelMap) {
		if (!rect.contains(mouseScreenPos.x, mouseScreenPos.y)) continue;
		if (!canPlaceItem(entity)) continue;

		auto type = entity->getType();
		if (type != EntityType::type_potion
			&& type != EntityType::type_scroll
			&& type != EntityType::type_book) continue;

		entity->use();

		// Освобождаем ячейки
		if (type == EntityType::type_book) {
			for (int row = 0; row < m_maxRowIP; ++row) {
				for (int col = 0; col < m_maxColIP; ++col) {
					if (rect.intersects(m_cellIP[row][col]->m_cellRect)) {
						m_cellIP[row][col]->m_free = true;
					}
				}
			}
		}
		else {
			for (int i = 0; i < m_maxColIP; ++i) {
				if (rect == m_cellIP[3][i]->m_cellRect) {
					m_cellIP[3][i]->m_free = true;
					break;
				}
			}
		}

		toRemove.push_back(entity);
		break;
	}

	for (auto* e : toRemove)
		m_inventoryPanelMap.erase(e);
}

// =========================================================================
// Настройка инвентаря (взять / положить / выбросить)
// =========================================================================

void Inventory::setupInventoryIP() {
	// TODO: получение позиции мыши и проверка нажатия
	sf::Vector2i mousePos{ 0, 0 };
	bool leftMousePressed = false;

	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return;                            // защита от nullptr
	//Player* player = playerPtr.get();                       // получаем сырой указатель
	Player* player = m_context.m_playerPtr;
	if (!player) return;

	// TODO: проверка m_cycleCounter для debounce

	// Выбрасывание предмета
	// TODO: if (!m_context.m_mHandlerPtr->clickingPanels() && m_largeCursorInstalled && leftMousePressed) { ... }

	// Взятие предмета
	if (!m_largeCursorInstalled && leftMousePressed) {
		for (auto&[entity, rect] : m_inventoryPanelMap) {
			if (!rect.contains(mousePos.x, mousePos.y)) continue;
			if (entity->getType() == EntityType::type_cash) continue;
			if (rect == m_shieldCell->m_cellRect && entity->isTwoHandedWeapon()) continue;

			// TODO: m_context.m_cPanelPtr->soundTakingItem(entity);
			// TODO: m_context.m_cPanelPtr->setGlobalEntity(entity);
			entity->setInInventory(false);
			m_largeCursorInstalled = true;

			auto& eq = player->getEquipment();

			if (rect == m_weaponsCell->m_cellRect) {
				setTwoHandedWeapons(entity, false);
				m_weaponsCell->m_free = true;
				if (eq.m_weapon) eq.m_weapon->setDressed(false);
				// TODO: recalculateData
				eq.m_weapon = nullptr;
				// TODO: changeAnimationType
			}
			else if (rect == m_shieldCell->m_cellRect) {
				m_shieldCell->m_free = true;
				if (eq.m_shield) eq.m_shield->setDressed(false);
				eq.m_shield = nullptr;
				// TODO: changeAnimationType
			}
			else if (rect == m_helmetCell->m_cellRect) {
				m_helmetCell->m_free = true;
				if (eq.m_helmet) eq.m_helmet->setDressed(false);
				eq.m_helmet = nullptr;
			}
			else if (rect == m_armorCell->m_cellRect) {
				m_armorCell->m_free = true;
				if (eq.m_armor) eq.m_armor->setDressed(false);
				eq.m_armor = nullptr;
				// TODO: changeAnimationType
			}
			else if (rect == m_amuletCell->m_cellRect) {
				m_amuletCell->m_free = true;
				if (eq.m_amulet) eq.m_amulet->setDressed(false);
				eq.m_amulet = nullptr;
			}
			else if (rect == m_leftRingCell->m_cellRect) {
				m_leftRingCell->m_free = true;
				if (eq.m_leftRing) eq.m_leftRing->setDressed(false);
				eq.m_leftRing = nullptr;
			}
			else if (rect == m_rightRingCell->m_cellRect) {
				m_rightRingCell->m_free = true;
				if (eq.m_rightRing) eq.m_rightRing->setDressed(false);
				eq.m_rightRing = nullptr;
			}
			else {
				// Общие ячейки — освобождаем пересекающиеся
				for (int row = 0; row < m_maxRowIP; ++row) {
					for (int col = 0; col < m_maxColIP; ++col) {
						if (rect.intersects(m_cellIP[row][col]->m_cellRect)) {
							m_cellIP[row][col]->m_free = true;
						}
					}
				}
			}
			break;
		}
	}

	// TODO: удаление globalEntity из карты
	// if (m_context.m_cPanelPtr->getGlobalEntity() != nullptr)
	//     m_inventoryPanelMap.erase(m_context.m_cPanelPtr->getGlobalEntity());

	// Обратный процесс — размещение
	// TODO: if (m_largeCursorInstalled && leftMousePressed)
	//     manageCurrentItems(m_context.m_cPanelPtr->getGlobalEntity());
}

// =========================================================================
// Удаление изношенных предметов
// =========================================================================

void Inventory::deleteUsedItems() {
	//const auto& playerPtr = m_context.m_playerPtr;          // ссылка на shared_ptr (дешевле)
	//if (!playerPtr) return;                                 // защита от nullptr
	//Player* player = playerPtr.get();
	
	Player* player = m_context.m_playerPtr;
	if (!player) return;

	auto& eq = player->getEquipment();

	auto checkAndRemove = [&](Entity*& equipped, Cell* cell) -> bool {
		if (!equipped) return false;

		const auto* stats = equipped->getItemStats();
		if (!stats) return false;              // защита от nullptr

		if (stats->m_durability > 0) return false;
		m_inventoryPanelMap.erase(equipped);
		// TODO: m_context.m_cPanelPtr->singlePlayback("Resources/Sound/player/item_unusable.wav");
		// player->addMessage(L"У предмета " + equipped->getName() + L" закончилась прочность");
		equipped = nullptr;
		if (cell) cell->m_free = true;
		return true;
	};

	// Двуручное оружие — удаляет и клон из щита
	if (eq.m_weapon && eq.m_weapon->getItemStats()->m_durability == 0 && eq.m_weapon->isTwoHandedWeapon()) {
		m_inventoryPanelMap.erase(eq.m_weapon);
		m_inventoryPanelMap.erase(eq.m_shield);
		// TODO: звук + сообщение
		eq.m_weapon = nullptr;
		eq.m_shield = nullptr;
		m_weaponsCell->m_free = true;
		m_shieldCell->m_free = true;
		// TODO: changeAnimationType
	}
	else {
		checkAndRemove(eq.m_weapon, m_weaponsCell);
		if (eq.m_weapon == nullptr) {
			// TODO: changeAnimationType
		}
	}

	checkAndRemove(eq.m_shield, m_shieldCell);
	checkAndRemove(eq.m_helmet, m_helmetCell);
	if (checkAndRemove(eq.m_armor, m_armorCell)) {
		// TODO: changeAnimationType
	}
	checkAndRemove(eq.m_amulet, nullptr);
	checkAndRemove(eq.m_leftRing, nullptr);
	checkAndRemove(eq.m_rightRing, nullptr);
}

// =========================================================================
// Update
// =========================================================================

void Inventory::update() {
	m_cycleCounter++;
	useItemIP();
	// deleteUsedItems(); // Запускается в UI
	setupInventoryIP();
}
