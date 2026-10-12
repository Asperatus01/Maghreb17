#pragma once

enum class AnimationState {
	Nothing,
	AttackingPose,
	Movement,
	Attacking,
	Destroy,
	Healing,
	Guarding,
	TakeDamage,
	Dying,
	Resurrection
};

