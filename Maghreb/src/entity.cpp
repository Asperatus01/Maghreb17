#include "entities/entity.hpp"
#include <iostream>

// Конструктор: принимает ссылку, инициализирует ссылку
Entity::Entity(SharedContext& context)
	: m_context(context)
	, m_position(0.f, 0.f)
	, m_solidArea(0.f, 0.f, 32.f, 32.f)
	, m_solidAreaDefault(m_solidArea)
	, m_type(EntityType::type_unknown)
	, m_collision(true)
	, m_name(L"")                 // <-- Важно: инициализируем пустым
	, m_itemStats(ItemStats{}) // <-- Создаём валидный объект сразу. 
							   // Теперь getItemStats() НИКОГДА не вернёт nullptr.           
{
	std::cerr << "[Stub] Entity created\n";
}

sf::FloatRect Entity::getWorldCoordSolid() const {
	return {
		m_position.x + m_solidArea.left,
		m_position.y + m_solidArea.top,
		m_solidArea.width,
		m_solidArea.height
	};
}

sf::Vector2f Entity::getCenter() const {
	return {
		m_position.x + m_solidArea.left + m_solidArea.width / 2.f,
		m_position.y + m_solidArea.top + m_solidArea.height / 2.f
	};
}

void Entity::getSprite(
	const sf::Texture& texture,
	unsigned int frameCol,
	unsigned int frameRow,
	sf::Sprite& outFirstFrame,
	sf::Sprite& outSecondFrame,
	sf::Sprite& outLastFrame
) {
	sf::Vector2u size = texture.getSize();
	int frameWidth = static_cast<int>(size.x) / static_cast<int>(frameCol);
	int frameHeight = static_cast<int>(size.y) / static_cast<int>(frameRow);

	// Первый кадр
	outFirstFrame.setTexture(texture, true);
	outFirstFrame.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

	// Второй кадр (если есть хотя бы два кадра)
	if (frameCol * frameRow > 1) {
		outSecondFrame.setTexture(texture, true);
		int secondIndex = 1;
		int columnIndex = secondIndex % frameCol;
		int rowIndex = secondIndex / frameCol;
		outSecondFrame.setTextureRect(sf::IntRect(
			columnIndex * frameWidth,
			rowIndex * frameHeight,
			frameWidth,
			frameHeight
		));
	}
	else {
		// Если кадр всего один, второй равен первому
		outSecondFrame = outFirstFrame;
	}

	// Последний кадр
	int totalFrames = static_cast<int>(frameCol * frameRow);
	if (totalFrames > 0) {
		int lastIndex = totalFrames - 1;
		int columnIndex = lastIndex % frameCol;
		int rowIndex = lastIndex / frameCol;

		outLastFrame.setTexture(texture, true);
		outLastFrame.setTextureRect(sf::IntRect(
			columnIndex * frameWidth,
			rowIndex * frameHeight,
			frameWidth,
			frameHeight
		));
	}
}

void Entity::getAnimation(
	const std::string& texturePath,
	unsigned int frameCol,
	unsigned int frameRow,
	std::vector<sf::Sprite>& outSpriteVect
) {
	const sf::Texture& tex = AssetManager::getTexture(texturePath); // твой проверенный вызов

	sf::Vector2u size = tex.getSize();
	int frameWidth = static_cast<int>(size.x) / static_cast<int>(frameCol);
	int frameHeight = static_cast<int>(size.y) / static_cast<int>(frameRow);

	outSpriteVect.clear();
	outSpriteVect.reserve(frameCol * frameRow);

	sf::Sprite frameSprite;
	frameSprite.setTexture(tex, true); // true — не копировать текстуру

	for (unsigned int i = 0; i < frameRow; ++i) {
		for (unsigned int j = 0; j < frameCol; ++j) {
			int x = static_cast<int>(j) * frameWidth;
			int y = static_cast<int>(i) * frameHeight;

			frameSprite.setTextureRect(sf::IntRect(x, y, frameWidth, frameHeight));
			outSpriteVect.push_back(frameSprite);
		}
	}
}

void Entity::setAnimationPointers(
	const sf::Sprite* firstFrame,
	const sf::Sprite* secondFrame,
	const sf::Sprite* lastFrame,
	const sf::Sprite* strokeSprite,
	const sf::Sprite* redSprite,
	const sf::Sprite* primarySprite
) {
	m_firstFramePtr = firstFrame;
	m_secondFramePtr = secondFrame;
	m_lastFramePtr = lastFrame;
	m_strokeSpritePtr = strokeSprite;
	m_redSpritePtr = redSprite;
	m_primarySpritePtr = primarySprite;
	m_outputSpritePtr = lastFrame; // по умолчанию показываем последний кадр
}

void Entity::setAnimationPointers(const sf::Sprite* staticSprite) {
	// Мы передаём staticSprite на место lastFrame.
	// Именно он станет m_outputSpritePtr.
	setAnimationPointers(nullptr, nullptr, staticSprite, nullptr, nullptr, nullptr);
}

// Новый метод: инкапсулируем доступ к аниматору
void Entity::activateAnimation(const std::string& name) {
	if (m_context.m_itemAnimHolderPtr) {
		m_context.m_itemAnimHolderPtr->setActiveAnimation(name);
	}
}

// Новый метод: даём доступ к AnimationHolder
AnimationHolder* Entity::getAnimatorHolder() const {
	return m_context.m_itemAnimHolderPtr;
}

void Entity::update(sf::Time dt) {
	// Заглушка
}

void Entity::interact() {
	// Пустая реализация
}

void Entity::use() {
	// Пустая реализация
}


void Entity::draw(sf::RenderTarget& target) const {
	// 1. Рисуем сам спрайт
	if (m_outputSpritePtr) {
		target.draw(*m_outputSpritePtr);
	}

	// 2. Отладочная отрисовка хитбокса
	// Используем макрос. В Debug-сборке он определен, в Release - нет.
#ifdef DEBUG_DRAW_HITBOX
	// Проверяем, включен ли режим отладки в контексте (дублируем логику на всякий случай)
	if (m_context.m_debugMode) {

		// Получаем позицию спрайта. 
		// ВАЖНО: Если ты меняешь позицию через m_position, а спрайт обновляешь отдельно,
		// бери позицию из спрайта, чтобы не рассинхронизировать.
		const sf::Vector2f pos = m_outputSpritePtr ? m_outputSpritePtr->getPosition() : sf::Vector2f(0, 0);

		// Создаем прямоугольник хитбокса
		sf::RectangleShape rect(sf::Vector2f(
			static_cast<float>(m_solidArea.width),
			static_cast<float>(m_solidArea.height)
		));

		// Сдвигаем его на позицию сущности + смещение внутри спрайта (m_solidArea.left/top)
		rect.setPosition(pos.x + static_cast<float>(m_solidArea.left),
			pos.y + static_cast<float>(m_solidArea.top));

		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineThickness(2.f);      // Чуть толще, чтобы было видно
		rect.setOutlineColor(sf::Color::Yellow); // Яркий цвет для отладки

		target.draw(rect);
	}
#endif
}
