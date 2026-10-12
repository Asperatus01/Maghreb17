#include "main/mouse_handler.hpp"
#include <iostream>

static constexpr int kTileSize = 32;

MouseHandler::MouseHandler(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] MouseHandler created\n";
}

MouseHandler::~MouseHandler() {
	std::cerr << "[Stub] MouseHandler destroyed\n";
	// TODO: evMgr->removeCallback(StateType::Game, "Left_Pressed")
	// TODO: evMgr->removeCallback(StateType::Game, "Right_Pressed")
}

void MouseHandler::onCreate() {
	std::cerr << "[Stub] MouseHandler::onCreate\n";
	// TODO: регистрация колбэков через EventManager
	// evMgr->addCallback(StateType::Game, "Left_Pressed", &MouseHandler::actionLeftButton, this);
	// evMgr->addCallback(StateType::Game, "Right_Pressed", &MouseHandler::actionRightButton, this);
}

void MouseHandler::actionLeftButton(EventDetails* /*details*/) {
	m_leftMousePressed = true;
	// TODO: полная логика левого клика
	// 1. Получить экранные координаты
	// 2. Перевести в мировые
	// 3. Проверить clickingPanels()
	// 4. Вызвать getSelectedEntity()
	// 5. Установить индекс цели игроку
	// 6. Обработка подбора предметов / атаки / движения
}

void MouseHandler::actionRightButton(EventDetails* /*details*/) {
	std::cerr << "[Stub] Right mouse button pressed\n";
	// TODO: логика правого клика
}

int MouseHandler::getSelectedEntity() {
	int i;
	if ((i = scanningNpc()) != 999)            return i;
	else if ((i = scanningObjects()) != 999)    return i;
	else if ((i = scanningInteractiveTile()) != 999) return i;
	else if ((i = scanningMonsters()) != 999)   return i;
	else return 999;
}

int MouseHandler::scanningMonsters() {
	if (m_leftMousePressed) return scanningMonstersLeft();
	else return scanningMonstersRight();
}

int MouseHandler::scanningInteractiveTile() {
	// TODO: сканирование m_context.m_aSetterPtr->m_iObjects
	// установка m_destroy = true при попадании клика
	return 999;
}

int MouseHandler::scanningMonstersLeft() {
	// TODO: сканирование m_context.m_aSetterPtr->m_monsters
	// установка m_attacking = true при попадании клика
	return 999;
}

int MouseHandler::scanningMonstersRight() {
	return 999;
}

int MouseHandler::scanningNpc() {
	// TODO: сканирование m_context.m_aSetterPtr->m_npc
	return 999;
}

int MouseHandler::scanningObjects() {
	// TODO: сканирование m_context.m_aSetterPtr->m_obj
	// установка m_pickUp = true при попадании клика
	return 999;
}

bool MouseHandler::getSolidTile() {
	// TODO: проверка тайла под курсором через TileManager
	// return false если тайл сплошной, true если проходимый
	return true;
}

sf::Vector2i MouseHandler::getPixelPos() const {
	// TODO: return m_context.m_eventManagerPtr->getMousePos(window)
	return { 0, 0 };
}

sf::Vector2i MouseHandler::getWorldPos(const sf::Vector2i& mousePos) const {
	// TODO: return window->mapPixelToCoords(mousePos)
	return mousePos;
}

bool MouseHandler::clickingPanels() {
	// TODO: проверка попадания клика на ControlPanel или InventoryPanel
	// return m_context.m_cPanelPtr->inBounds(...)
	//     || m_context.m_iPanelPtr->inBounds(...)
	return false;
}
