#include "main/animation_holder.hpp"
#include "main/animator.hpp"
#include "main/shared_context.hpp"
#include <iostream>

AnimationHolder::AnimationHolder()
	: m_sprite()
	, m_animator(m_sprite)
{
	// Конструктор только инициализирует и делегирует загрузку
	//changeAnimationPackage();
}

AnimationHolder::~AnimationHolder()
{
	// Текстуры живут в AssetManager (синглтон).
	// Мы храним только указатели, утечек нет.
}

void AnimationHolder::changeAnimationPackage()
{
	// --- ЗАГРУЗКА ВСЕХ АНИМАЦИЙ (ПАКЕТ) ---
	// Теперь не нужно передавать 96, 96 — размер кадра посчитается автоматически.

	loadSingleAnimation("sap", "res/player/rlb/rlbas.png", 8, 8, 12.0f, true);
	loadSingleAnimation("att", "res/player/rlb/rlbat.png", 12, 8, 12.0f, false);
	loadSingleAnimation("wap", "res/player/rlb/rlbaw.png", 8, 8, 12.0f, true);
	loadSingleAnimation("harm", "res/player/rlb/rlbht.png", 7, 8, 12.0f, false);
	loadSingleAnimation("stand", "res/player/rlb/rlbst.png", 20, 8, 12.0f, true);
	loadSingleAnimation("walk", "res/player/rlb/rlbwl.png", 8, 8, 12.0f, true);
	loadSingleAnimation("fma", "res/player/rlb/rlbfm.png", 16, 8, 12.0f, false);
	loadSingleAnimation("qma", "res/player/rlb/rlbqm.png", 16, 8, 12.0f, false);
	loadSingleAnimation("lma", "res/player/rlb/rlblm.png", 16, 8, 12.0f, false);
	loadSingleAnimation("death", "res/player/rln/rlndt.png", 20, 8, 12.0f, false);

	// По умолчанию устанавливаем анимацию "sap"
	setActiveAnimation("sap");
}

void AnimationHolder::loadSingleAnimation(
	const std::string& key,
	const std::string& path,
	int cols,
	int rows,
	float fps,
	bool loop
)
{
	try
	{
		// 1. Получаем текстуру
		sf::Texture& textureRef = AssetManager::getTexture(path);
		m_textures[key] = &textureRef;

		// 2. Считаем размер одного кадра
		auto texSize = textureRef.getSize();
		int frameWidth = static_cast<int>(texSize.x) / cols;
		int frameHeight = static_cast<int>(texSize.y) / rows;

		// 3. Создаём анимацию
		// Внимание: если createAnimation раньше принимал duration, теперь его нужно тоже переделать!
		auto& anim = m_animator.createAnimation(key, cols, rows, fps, loop);

		// Записываем размер кадра и сетку
		anim.m_frameSize = { frameWidth, frameHeight };
		anim.m_cols = static_cast<unsigned int>(cols);
		anim.m_rows = static_cast<unsigned int>(rows);

		// 4. Готовим полный список кадров (атлас) и сохраняем его
		const std::vector<sf::IntRect>& fullFrames = AssetManager::prepareAtlasFrames(
			path,
			sf::Vector2i(frameWidth, frameHeight),
			cols,
			rows
		);

		anim.m_framesStorage = fullFrames;

		// ВАЖНО: НИЧЕГО НЕ ПИШЕМ В anim.m_frames!
		// Он должен остаться пустым до вызова loadDirectionalAnimation.

		std::cout << "[AnimationHolder] Loaded: " << key
			<< " (frame: " << frameWidth << "x" << frameHeight
			<< ", fps: " << fps << ")\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "[ERROR] Failed to load '" << key << "': " << e.what() << "\n";
	}
}
// Загрузка простой однострочной анимации
void AnimationHolder::loadSimpleAnimation(
	const std::string& name,       // "bow_drop"
	const std::string& path,        // "Resources/Items/bow.png"
	int framesCount,                // 14
	float fps,                      // 12.0
	bool loop                       // false
) {
	loadSingleAnimation(name, path, framesCount, 1, fps, loop);
}

void AnimationHolder::setActiveAnimation(const std::string& type)
{
	auto it = m_textures.find(type);
	if (it == m_textures.end())
	{
		std::cerr << "[WARNING] Animation '" << type << "' not found.\n";
		return;
	}

	m_currentType = type;

	// 1. Ставим текстуру (true = не копировать текстуру, просто привязать)
	m_sprite.setTexture(*it->second, true);

}

std::string AnimationHolder::getCurrentType() const
{
	return m_currentType;
}

sf::Sprite& AnimationHolder::getSprite()
{
	return m_sprite;
}

Animator& AnimationHolder::getAnimator()
{
	return m_animator;
}
