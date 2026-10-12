#pragma once

#include "panels/panel.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Entity;

class ControlPanel : public Panel {
public:
	explicit ControlPanel(SharedContext& context);
	~ControlPanel() override;

	void onCreate() override;
	void loadResources() override;
	void createPanelButton() override;
	void displayItems() override;
	void drawPanels();

	void drawLifeMana();
	void drawLife();
	void drawMana();
	void drawPressedButton();

	void update() override;
	void draw() override;

	void getInfo(std::vector<Entity*> entityVec);
	void getInfoCP();
	void useItemsCP();
	void setupInventoryCP();

	void soundTakingItem(Entity* entity);
	void soundAddingItem(Entity* entity);
	void addToCell(Entity* entity, int i);

	Entity* getGlobalEntity() const;
	void setGlobalEntity(Entity* globalEntity);

	void singlePlayback(const std::string& filename);

	bool m_displayInventary = false;

private:
	// Текстуры панели
	sf::Texture m_textPanel, m_textPanelLeft, m_textPanelRight, m_textPanelBoth;
	sf::Texture m_textBulbLeft, m_textBulbRight, m_textSegment;
	sf::Texture m_textChar, m_textQuests, m_textMap, m_textMenu, m_textInv, m_textSpells;

	// Спрайты панели
	sf::Sprite m_panel, m_panelLeft, m_panelRight, m_panelBoth;
	sf::Sprite m_bulbLeft, m_bulbRight, m_segment;
	sf::Sprite m_char, m_quests, m_map, m_menu, m_inv, m_spells;

	// Прямоугольники кнопок
	sf::IntRect m_charRect, m_questsRect, m_mapRect, m_menuRect;
	sf::IntRect m_invRect, m_spellsRect, m_spellIconRect;
	sf::IntRect m_bulbRect;

	// Текст
	sf::Font m_font;
	sf::Text m_information;

	// Глобальная сущность (предмет на курсоре)
	Entity* m_globalEntity = nullptr;

	// Счётчики
	int m_cycleCounter = 0;
	int m_soundPlayerCounter = 0;
	bool m_hasBeenPlayed = false;
};

