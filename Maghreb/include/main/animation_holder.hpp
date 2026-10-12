#pragma once

#include "entities/entity.hpp"
#include "shared_context.hpp"

class AnimationHolder : public Entity {
public:
	explicit AnimationHolder(SharedContext& context);
	~AnimationHolder() override = default;

	void changeAnimationType();
	void recalculateData(Entity* currentItem);

private:
	SharedContext& m_context;
};

