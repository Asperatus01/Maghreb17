#pragma once

#include <string>
#include <unordered_map>
#include "animator.hpp"
#include "asset_manager.hpp"

// Forward declaration, чтобы не т€нуть лишние заголовки сюда
class sf::Sprite;

class AnimationHolder
{
public:
	AnimationHolder();
	~AnimationHolder();

	// ѕереключить на нужную анимацию по ключу (sap, att, stand и т.д.)
	void setActiveAnimation(const std::string& type);

	// ѕолучить текущий активный тип (дл€ отладки или логики)
	std::string getCurrentType() const;

	// ќсновной метод дл€ загрузки/переключени€ пакета анимаций.
	// —ейчас он загружает все анимации проекта и устанавливает начальную ("sap").
	// ¬ будущем сюда добавишь логику выбора в зависимости от экипировки.
	void changeAnimationPackage();

	// ¬ернуть ссылку на спрайт, которым управл€ет этот держатель
	sf::Sprite& getSprite();

	// ¬ернуть ссылку на Animator, чтобы обновл€ть кадры
	Animator& getAnimator();

private:
	sf::Sprite m_sprite;
	Animator m_animator;

	// ћапы дл€ хранени€ данных по каждой анимации
	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::unordered_map<std::string, std::vector<sf::IntRect>> m_framesMap;

	// “екущий активный тип
	std::string m_currentType;

	// «агрузка одной анимации
	void loadSingleAnimation(
		const std::string& key,
		const std::string& path,
		int cols,
		int rows,
		float fps,
		bool loop
	);

	// «агрузка простой (однострочной) анимации
	void loadSimpleAnimation(
		const std::string& name,        // "bow_drop"
		const std::string& path,        // "Resources/Items/bow.png"
		int framesCount,                // 14
		float fps,                      // 12.0
		bool loop);
};
