#include "main/animation_holder.hpp"
#include "main/animator.hpp"
#include "entities/player.hpp"
#include <iostream>

AnimationHolder::AnimationHolder(SharedContext& context)
	: Entity(context)
	, m_context(context) {}

void AnimationHolder::changeAnimationType() {
	auto* player = m_context.m_playerPtr;
	if (!player) {
		return;
	}

	auto* animator = player->GetAnimator();
	if (!animator) {
		return;
	}

	// Вспомогательная лямбда, чтобы не дублировать CreateAnimation
	auto createAnim = [&](const std::string& name, const std::string& path,
		int framesX, int framesY, float durationSec, bool loop) {
		animator->CreateAnimation(name, path, framesX, framesY, sf::seconds(durationSec), loop);
	};

	const auto* armor = player->m_currentArmor;
	const bool isLightArmor = (!armor || armor->GetType() == EntityType::type_lightArmor);
	const auto* weapon = player->m_currentWeapon;
	const auto* shield = player->m_currentShield;

	if (isLightArmor) {
		if (!weapon || (weapon->GetType() == EntityType::type_hands && !shield)) {
			// Без оружия
			createAnim("sap", "Resources/player/rln/rlnas.png", 8, 8, 1.5f, true);
			createAnim("att", "Resources/player/rln/rlnat.png", 18, 8, 1.5f, false);
			createAnim("wap", "Resources/player/rln/rlnaw.png", 8, 8, 1.0f, true);
			createAnim("harm", "Resources/player/rln/rlnht.png", 7, 8, 1.0f, false);
			createAnim("stand", "Resources/player/rln/rlnst.png", 20, 8, 1.0f, true);
			createAnim("walk", "Resources/player/rln/rlnwl.png", 8, 8, 1.0f, true);
			createAnim("fma", "Resources/player/rln/rlnfm.png", 16, 8, 1.5f, false);
			createAnim("qma", "Resources/player/rln/rlnqm.png", 16, 8, 1.5f, false);
			createAnim("lma", "Resources/player/rln/rlnlm.png", 16, 8, 1.5f, false);
			createAnim("death", "Resources/player/rln/rlndt.png", 20, 8, 2.0f, false);
		}
		else if (weapon == shield) {
			// Только щит
			createAnim("sap", "Resources/player/rlu/rluas.png", 8, 8, 1.5f, true);
			createAnim("att", "Resources/player/rlu/rluat.png", 18, 8, 1.5f, false);
			createAnim("wap", "Resources/player/rlu/rluaw.png", 8, 8, 1.0f, true);
			createAnim("shp", "Resources/player/rlu/rlubl.png", 4, 8, 1.0f, false);
			createAnim("harm", "Resources/player/rlu/rluht.png", 7, 8, 1.0f, false);
			createAnim("stand", "Resources/player/rlu/rlust.png", 20, 8, 1.0f, true);
			createAnim("walk", "Resources/player/rlu/rluwl.png", 8, 8, 1.0f, true);
			createAnim("fma", "Resources/player/rlu/rlufm.png", 16, 8, 1.5f, false);
			createAnim("qma", "Resources/player/rlu/rluqm.png", 16, 8, 1.5f, false);
			createAnim("lma", "Resources/player/rlu/rlulm.png", 16, 8, 1.5f, false);
			createAnim("death", "Resources/player/rln/rlndt.png", 20, 8, 2.0f, false);
		}
		else {
			if (weapon->GetType() == EntityType::type_bow) { // Лук
				createAnim("sap", "Resources/player/rlb/rlbas.png", 8, 8, 1.5f, true);
				createAnim("att", "Resources/player/rlb/rlbat.png", 12, 8, 1.5f, false);
				createAnim("wap", "Resources/player/rlb/rlbaw.png", 8, 8, 1.0f, true);
				createAnim("harm", "Resources/player/rlb/rlbht.png", 7, 8, 1.0f, false);
				createAnim("stand", "Resources/player/rlb/rlbst.png", 20, 8, 1.0f, true);
				createAnim("walk", "Resources/player/rlb/rlbwl.png", 8, 8, 1.0f, true);
				createAnim("fma", "Resources/player/rlb/rlbfm.png", 16, 8, 1.5f, false);
				createAnim("qma", "Resources/player/rlb/rlbqm.png", 16, 8, 1.5f, false);
				createAnim("lma", "Resources/player/rlb/rlblm.png", 16, 8, 1.5f, false);
				createAnim("death", "Resources/player/rln/rlndt.png", 20, 8, 2.0f, false);
			}
			else if (weapon->GetType() == EntityType::type_axe) {
				// ... аналогично для остальных типов оружия (сокращено для примера)
				// Здесь ты можешь просто перенести свои ветки из старого кода
			}
			// И так далее для остальных веток
		}
	}
}

void AnimationHolder::recalculateData(Entity* /*currentItem*/) {
	// Сейчас метод пустой. Если позже понадобится — раскомментируй нужную логику.
	// Пока оставим заглушку, чтобы не тащить 50 строк закомментированного кода.
}
