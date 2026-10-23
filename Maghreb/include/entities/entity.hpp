#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "main/shared_context.hpp"
#include "objects/item_stats.hpp"
#include "main/animation_holder.hpp"
#include "sound/sound_player.hpp"

enum class EntityType {
	type_unknown,
	type_hands,
	type_sword,
	type_axe,
	type_shield,
	type_helmet,
	type_bow,
	type_arrow,
	type_firebolt,
	type_gold,
	type_potion,
	type_scroll,
	type_lightArmor,
	type_middleArmor,
	type_heavyArmor,
	type_staff,
	type_mace,
	type_amulet,
	type_barrel,
	type_obstacle,// тип препятствие
	type_landscape,
	type_ring,
	type_book,
	type_cash,
	type_chargedbolt,
	type_healing,
	type_lighting,
	type_architecture,
	type_magic,
	type_monster,
	type_player,
	type_npc,
	type_item,
	type_interactive
};

class Entity {
public:
	explicit Entity(SharedContext& context);
	virtual ~Entity() = default;

	// --- Имя / Уникальный ID (новое) ---
	const std::wstring& getName() const { return m_name; }
	void setName(const std::wstring& name) { m_name = name; }
	void setName(const std::string& nameUtf8) {
		// Если CSV в UTF-8, а нужно wstring — можно добавить простую конвертацию, 
		// но пока оставим только wstring-версию для простоты.
		m_name = std::wstring(nameUtf8.begin(), nameUtf8.end());
	}
	   
	sf::Vector2f getPosition() const { return m_position; }

	// ВАЖНО: virtual здесь обязателен!
	virtual void setPosition(const sf::Vector2f& pos) { m_position = pos; };

	// Если у тебя есть перегрузка с float x, y — тоже сделай её virtual
	virtual void setPosition(float x, float y) { m_position = { x, y }; };


	sf::FloatRect getSolidArea() const { return m_solidArea;	}
	void setSolidArea(const sf::FloatRect& area) {	m_solidArea = area;	}

	sf::FloatRect getWorldCoordSolid() const;
	sf::Vector2f getCenter() const;

	EntityType getType() const { return m_type;	}
	void setType(EntityType type) {	m_type = type; }

	/*EntityType getType() const { return m_type; }
	const wchar_t* getName() const { return m_name.c_str(); }*/

	bool getCollision() const {	return m_collision;	}
	void setCollision(bool collision) {	m_collision = collision; }

	SharedContext& getContext() const {	return m_context; }

	// ВАЖНО: теперь возвращаем указатель, а не ссылку, так как статы может и не быть
	ItemStats* getItemStats() {
		return m_itemStats.has_value() ? &m_itemStats.value() : nullptr;
	}

	const ItemStats* getItemStats() const {
		return m_itemStats.has_value() ? &m_itemStats.value() : nullptr;
	}

	void setInInventory(bool value) { m_inInventory = value; }
	bool isInInventory() const { return m_inInventory; }

	void setDressed(bool value) { m_dressed = value; }
	bool isDressed() const { return m_dressed; }

	void setTwoHandedWeapon(bool value) { m_twoHandedWeapon = value; }
	bool isTwoHandedWeapon() const { return m_twoHandedWeapon; }

	void setShowItem(bool value) { m_showItem = value; }
	bool isShowItem() const { return m_showItem; }

	void setOpened(bool value) { m_opened = value; }
	bool isOpened() const { return m_opened; }
		
	virtual void activateAnimation(const std::string& name);
	virtual AnimationHolder* getAnimatorHolder() const;
	
	// НОВЫЙ МЕТОД: помощник для проигрывания звука
	void playSound(const std::string& path) {
		if (m_context.m_soundPlayerPtr) {
			m_context.m_soundPlayerPtr->play(path);
		}
	}

	// Проверка попадания: точка в хитбоксе?
	// const — потому что проверка не должна менять состояние
	virtual bool contains(const sf::Vector2f& pos) const { return false; };

	// Реакция на клик: тут можно менять состояние (флаг clicked, прочность и т.п.)
	virtual void onMouseClick(const sf::Vector2f& pos) {};

	void getSprite(
		const sf::Texture& texture,
		unsigned int frameCol,
		unsigned int frameRow,
		sf::Sprite& outFirstFrame,
		sf::Sprite& outSecondFrame,
		sf::Sprite& outLastFrame
	);

	void getAnimation(
		const std::string& texturePath,
		unsigned int frameCol,
		unsigned int frameRow,
		std::vector<sf::Sprite>& outSpriteVect);

	void setAnimationPointers(
		const sf::Sprite* firstFrame,
		const sf::Sprite* secondFrame,
		const sf::Sprite* lastFrame,
		const sf::Sprite* strokeSprite,
		const sf::Sprite* redSprite,
		const sf::Sprite* primarySprite
	);

	void setAnimationPointers(const sf::Sprite* staticSprite);

	virtual void update(sf::Time dt);
	virtual void draw(sf::RenderTarget& target) const;
	virtual void interact();
	virtual void use();

	// Animation
    // ВСЕ указатели — на const sf::Sprite. Это правильно.
	const sf::Sprite* m_strokeSpritePtr = nullptr;
	const sf::Sprite* m_lastFramePtr = nullptr;
	const sf::Sprite* m_outputSpritePtr = nullptr;
	const sf::Sprite* m_primarySpritePtr = nullptr;
	const sf::Sprite* m_redSpritePtr = nullptr;
	const sf::Sprite* m_firstFramePtr = nullptr;
	const sf::Sprite* m_secondFramePtr = nullptr;

protected:
	SharedContext& m_context;      // указатель вместо ссылки
	sf::Vector2f m_position;
	sf::FloatRect m_solidArea;
	sf::FloatRect m_solidAreaDefault;
	EntityType m_type = EntityType::type_unknown;
	
    sf::FloatRect m_boundingBox; // Рамка ограничивающая спрайт
    bool m_outline = false;      // Мышь навели на предмет
	
	bool m_inInventory = false;             // Предмет находится в инвентаре
	bool m_dressed = false;                 // Предмет экипировки надет 
	bool m_showItem = false;                // Показываем объект, например выпрыгивание из сундука
	bool m_twoHandedWeapon = false;       // Двуручное оружие
	bool m_opened = false;                  // Открытие сундука
	bool m_collision = true;                // Произошло столкновение

	// Дополнительные флаги
	bool m_alive;             // эффект смерти монстра или целостнотности объекта
	bool m_dying;             // процесс умирания объекта или монстра
	bool m_canPlay;
	bool m_destroy;           // Флаг устанавливается если игрок рубит дерево
	bool m_destroyed;         // уничтоженный объект
	bool m_deceased;          // умерший
	bool m_shootArrow;
	bool m_shootMagic;
	//bool m_imageItemUploaded;   // Флаг, обозначающий, что вместо указателя мыши загружено изображение предмета
	bool m_showParticles;       // Включаем анимацию брызг крови при рубке деревьев
	

	// Имя = уникальный идентификатор (из CSV)
	std::wstring m_name;
private:
	std::optional<ItemStats>  m_itemStats;  // Экземпляр класса данных инвентаря
};
