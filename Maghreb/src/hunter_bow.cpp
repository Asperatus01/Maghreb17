#include <objects/hunter_bow.hpp>
#include <objects/sprite_effect.hpp>

HunterBow::HunterBow(SharedContext& context) : Bow(context)
{
	auto stats = getItemStats();
	m_name = L"Hunter bow";
	stats->m_description = L"Охотничий лук. Поражающая сила – 2-6, \nпрочность – 40,	\nтребования – 20 STR, 35 DEX";
	stats->m_price = 300;
	stats->m_attackSpeed = 5; // Скорость прокручивания анимации атаки, пока не реализовано
	stats->m_minDamage = 2;
	stats->m_maxDamage = 6;
	stats->m_durability = 40;
	stats->m_needStrength = 20;
	stats->m_needDexterity = 35;
	stats->m_knockBackPower = 100.f;

	// 1. Загружаем текстуру анимации
	m_texture = AssetManager::getTexture("res/items/bow.png");

	// 2. Вызываем getSprite с полными именами переменных.
	// Он не хранит ничего внутри себя — он просто пишет в наши локальные спрайты.
	getSprite(m_texture, 13, 1, m_firstFrame, m_secondFrame, m_lastFrame);

	// Текстура для отображения предмета в инвентаре
	m_primaryTexture = AssetManager::getTexture("res/objects/bow/Hunters Bow.png");
	// Заполняем m_primarySprite для отображения предмета в инвентаре
	m_primarySprite.setTexture(m_primaryTexture);

	m_outputSprite = m_lastFrame;

	// Параметры: sf::Sprite& outSprite, sf::Texture& outTexture, const sf::Sprite& source
	SpriteEffect::createStrokedSprite(m_strokeSprite, m_strokeTexture, m_lastFrame);

	// Параметры: sf::Sprite& outSprite, sf::Texture& outTexture, const sf::Texture& source
	SpriteEffect::createRedSprite(m_redSprite, m_redTexture, m_texture);

	// 5. Связываем указатели родителя с нашими объектами.
	setAnimationPointers(
		&m_firstFrame,
		&m_secondFrame,
		&m_lastFrame,
		&m_strokeSprite,
		&m_redSprite,
		&m_primarySprite
	);
	
}

HunterBow::~HunterBow() {}

