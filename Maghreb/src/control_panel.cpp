#include "panels/control_panel.hpp"
#include "entities/entity.hpp"
#include <iostream>

static constexpr int kScreenWidth = 800;
static constexpr int kScreenHeight = 600;

ControlPanel::ControlPanel(SharedContext& context)
	: Panel(context)
{
	std::cerr << "[Stub] ControlPanel created\n";
}

ControlPanel::~ControlPanel() {
	std::cerr << "[Stub] ControlPanel destroyed\n";
}

void ControlPanel::onCreate() {
	std::cerr << "[Stub] ControlPanel::onCreate\n";
	// TODO: loadResources() + позиционирование + шрифт
}

void ControlPanel::loadResources() {
	std::cerr << "[Stub] ControlPanel::loadResources\n";
	// TODO: загрузка текстур через AssetManager
}

void ControlPanel::createPanelButton() {
	std::cerr << "[Stub] ControlPanel::createPanelButton\n";
	// TODO: расстановка IntRect дл€ кнопок
}

void ControlPanel::displayItems() {
	// TODO: отрисовка предметов из инвентар€ CP
}

void ControlPanel::drawPanels() {
	// TODO: выбор панели (полна€/нет жизни/нет маны) и отрисовка
}

void ControlPanel::drawLifeMana() {
	// TODO: отрисовка панели с пустыми колбами жизни и маны
}

void ControlPanel::drawLife() {
	// TODO: отрисовка панели с пустой колбой жизни
}

void ControlPanel::drawMana() {
	// TODO: отрисовка панели с пустой колбой маны
}

void ControlPanel::drawPressedButton() {
	// TODO: подсветка нажатых кнопок панели
}

void ControlPanel::update() {
	m_cycleCounter++;
	if (m_hasBeenPlayed) {
		m_soundPlayerCounter++;
		if (m_soundPlayerCounter >= 10) {
			m_hasBeenPlayed = false;
		}
	}
	// TODO: useItemsCP(), setupInventoryCP(), обработка клавиш I/Q
}

void ControlPanel::draw() {
	// TODO: drawPanels(), displayItems(), getInfoCP(), getInfo() дл€ всех векторов
}

void ControlPanel::getInfo(std::vector<Entity*> /*entityVec*/) {
	// TODO: отображение описани€ сущности под курсором мыши
}

void ControlPanel::getInfoCP() {
	// TODO: отображение описани€ предмета из инвентар€ CP
}

void ControlPanel::useItemsCP() {
	// TODO: использование предмета по правому клику
}

void ControlPanel::setupInventoryCP() {
	// TODO: перемещение предметов между €чейками инвентар€ CP
}

void ControlPanel::soundTakingItem(Entity* /*entity*/) {
	// TODO: singlePlayback("Resources/Sound/items/invgrab.wav")
}

void ControlPanel::soundAddingItem(Entity* /*entity*/) {
	// TODO: звук в зависимости от типа предмета
}

void ControlPanel::addToCell(Entity* /*entity*/, int /*i*/) {
	// TODO: размещение предмета в €чейке инвентар€
}

Entity* ControlPanel::getGlobalEntity() const {
	return m_globalEntity;
}

void ControlPanel::setGlobalEntity(Entity* globalEntity) {
	m_globalEntity = globalEntity;
}

void ControlPanel::singlePlayback(const std::string& /*filename*/) {
	// TODO: воспроизведение звука через SoundPlayer
}
