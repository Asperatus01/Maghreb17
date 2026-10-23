#include "objects/bow.hpp"
#include "objects/item_stats.hpp" // Подключаем, если нужны типы, но сам класс ItemStats у тебя уже есть в заголовке Entity или Bow

Bow::Bow(SharedContext& context)
	: Entity(context)  // <-- Инициализирует m_context внутри Entity
{
	// 1. Общие данные для всех луков (из логики ItemStats)
	m_type = EntityType::type_bow;

	auto stats = getItemStats();
	// Заполняем структуру статов 
	stats->m_neededCells = 6;
	stats->m_width = 56;
	stats->m_height = 87;
	stats->m_knockBackPower = 1;

	// Специфичные флаги самого класса Bow
	m_twoHandedWeapon = true;
	m_collision = false;

	// Хитбокс (Solid Area) - тоже общий для всех базовых луков
	m_solidArea.left = 29;
	m_solidArea.top = 140;
	m_solidArea.width = 20;
	m_solidArea.height = 10;

	// 2. Активация анимации (НЕ загрузка!)
	// Всё, что нужно: говорим родителю «включи анимацию»
	activateAnimation("bow");
}


Bow::~Bow() {}


void Bow::update(const sf::Time dT) {
	if (m_showItem && !m_inInventory) {
		// Твоя проверенная формула движения
		m_position.y += rand() % 2 + 2;
		m_position.x += rand() % 2 - 2;

		// Цепочка доступа к флагу окончания анимации
		AnimationHolder* holder = getAnimatorHolder();
		if (holder) {
			// Получаем ссылку на Animator из AnimationHolder
			Animator& anim = holder->getAnimator();

			if (anim.getEndAnim()) {
				// Анимация закончилась — один раз играем звук
				// Играем звук через метод родителя (теперь m_context не нужен!)
				playSound("res/sound/items/flipbow.wav");

				// КРИТИЧЕСКИ ВАЖНО: сбрасываем флаг, чтобы звук не играл каждый кадр
				// Если у Animator нет публичного метода сброса, а m_endAnim публичный — можно написать anim.m_endAnim = false;
				// Но лучше добавить метод void resetEndAnim() { m_endAnim = false; } в класс Animator
				anim.resetEndAnim();

				m_showItem = false; // Лук теперь лежит

				// (Опционально) Переключаем на анимацию покоя
				// holder->setActiveAnimation("bow_idle");
			}
		}
	}
	else if (m_inInventory) { // Если предмет в инвентаре
		m_solidArea = { 0, 0, 64, 96 };
	}
	else {
		// Хитбоксы одинаковые для всех наследников
		// Если предмет вне инвентаря (он может быть выброшен из инвентаря)
		m_solidArea = { 29, 140, 20, 10 };
		
		// ГЛАВНАЯ ЛОГИКА ОБВОДКИ:
		// Если мышь наведена (m_outline == true) -> рисуем обводку
		// Иначе -> рисуем обычный спрайт
		if (m_outline) {
			m_outputSpritePtr = m_strokeSpritePtr;
		}
		else {
			m_outputSpritePtr = m_lastFramePtr;
		}
	}
}

void Bow::draw(sf::RenderTarget& target) {
	Entity::draw(target); // Вызываем базовую отрисовку спрайта
}
	



