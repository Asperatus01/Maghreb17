#include "main/animator.hpp"
#include <iostream>
#include <algorithm>
#include <optional>

void Animator::loadAnimation(const std::string& textureName,
	unsigned int frameCol, unsigned int frameRow) {

	// 1. Формируем полный путь с учетом переименованной папки
	std::string fullPath = "res/player/rlb" + textureName;

	sf::Texture texture;
	if (!texture.loadFromFile(fullPath)) {
		std::cerr << "[ERROR] Failed to load texture: " << fullPath << "\n";
		return;
	}

	sf::Vector2u texSize = texture.getSize();
	sf::Vector2u frameSize{ texSize.x / frameCol, texSize.y / frameRow };

	// Проверка на целостность (чтобы не было дробных пикселей)
	if (frameSize.x == 0 || frameSize.y == 0) {
		std::cerr << "[ERROR] Invalid frame size calculation.\n";
		return;
	}

	Animation anim;
	anim.m_name = textureName; // Используем имя файла как имя анимации
	anim.m_frameCol = frameCol;
	anim.m_frameRow = frameRow;
	anim.m_frameSize = frameSize;
	// Длительность кадра: общее время анимации / кол-во кадров. 
	// Для примера ставим 0.1 сек на кадр (10 FPS). Можно вынести в параметры.
	anim.m_frameDuration = 0.1f;

	// 2. Нарезаем спрайт-лист на отдельные кадры
	for (unsigned int r = 0; r < frameRow; ++r) {
		for (unsigned int c = 0; c < frameCol; ++c) {
			sf::Sprite sprite(texture);

			// Устанавливаем прямоугольник текстуры (нарезаем кадр)
			sprite.setTextureRect(sf::IntRect(
				static_cast<int>(c * frameSize.x),
				static_cast<int>(r * frameSize.y),
				static_cast<int>(frameSize.x),
				static_cast<int>(frameSize.y)
			));

			// ВАЖНО: Если картинка без прозрачности (как на твоем скриншоте),
			// белый фон будет виден. В идеале используй PNG с альфа-каналом.
			// Если нужно программно убрать белый цвет, потребуется шейдер или 
			// предварительная обработка текстуры (setMask).

			anim.m_frames.push_back(sprite);
		}
	}

	m_animations.push_back(anim);
	std::cout << "[INFO] Loaded animation '" << anim.m_name
		<< "' with " << anim.m_frames.size() << " frames.\n";
}

void Animator::update(sf::Time dt) {
	if (m_stopAnimation || m_animations.empty()) return;

	float deltaTime = dt.asSeconds();
	m_elapsed += deltaTime;

	const Animation& currentAnim = m_animations[m_currentAnim];

	// Если прошло время одного кадра
	if (m_elapsed >= currentAnim.m_frameDuration) {
		m_elapsed -= currentAnim.m_frameDuration;

		// Переключаем кадр
		m_currentFrame++;

		// Зацикливаем анимацию
		if (m_currentFrame >= currentAnim.m_frames.size()) {
			m_currentFrame = 0;
		}
	}
}

void Animator::draw(sf::RenderTarget& target, const sf::Vector2f& position) {
	if (m_animations.empty()) return;

	const Animation& currentAnim = m_animations[m_currentAnim];

	// Рисуем текущий кадр
	sf::Sprite& sprite = const_cast<sf::Sprite&>(currentAnim.m_frames[m_currentFrame]);
	sprite.setPosition(position);

	target.draw(sprite);
}

void Animator::switchAnimation(const std::string& name) {
	auto idx = findAnimationIndex(name);
	if (idx.has_value()) {
		m_currentAnim = idx.value();
		m_currentFrame = 0; // Сброс на первый кадр
		m_elapsed = 0.f;
		std::cout << "[INFO] Switched animation to: " << name << "\n";
	}
	else {
		std::cerr << "[WARN] Animation '" << name << "' not found!\n";
	}
}

std::string Animator::getCurrentAnimationName() const {
	if (m_animations.empty()) return "";
	return m_animations[m_currentAnim].m_name;
}

unsigned int Animator::getCurrentFrame() const {
	return m_currentFrame;
}

sf::Vector2u Animator::getFrameSize() const {
	if (m_animations.empty()) return { 0, 0 };
	return m_animations[m_currentAnim].m_frameSize;
}

bool Animator::isStopped() const { return m_stopAnimation; }
void Animator::setStopped(bool stopped) { m_stopAnimation = stopped; }


std::optional<size_t> Animator::findAnimationIndex(const std::string& name) const {
	for (size_t i = 0; i < m_animations.size(); ++i) {
		if (m_animations[i].m_name == name) {
			return i;
		}
	}
	return std::nullopt;
}

