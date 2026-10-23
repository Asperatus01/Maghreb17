#include "main/animator.hpp"
#include <cassert>
#include <iostream>

Animator::Animator(sf::Sprite& sprite)
	: m_sprite(sprite)
	, m_currentAnimation(nullptr)
	, m_stopAnimation(false)
	, m_endAnim(false)
	, m_currentDirection(Direction::Down)  // <-- Только тут
{
}

Animator::Animation& Animator::createAnimation(std::string const& name,
	unsigned int cols,
	unsigned int rows,
	float fps,
	bool looping)
{
	// ЭТА СТРОКА создаёт новый объект в map, если его ещё нет,
	// или возвращает ссылку на существующий, если ключ уже есть.
	auto& anim = m_animations[name];

	// Явно перезаписываем поля. Это гарантирует, что у каждой анимации
	// будут свои значения, даже если объект уже существовал.
	anim.m_name = name;
	anim.m_cols = cols;
	anim.m_rows = rows;
	anim.m_fps = fps;          // <-- вот тут сохраняется fps для ЭТОЙ анимации
	anim.m_looping = looping;

	return anim;
}


void Animator::loadDirectionalAnimation(const std::string& name)
{
	auto it = m_animations.find(name);
	if (it == m_animations.end()) {
		std::cerr << "[Animator] Error: animation '" << name << "' not found!\n";
		return;
	}

	Animation& anim = it->second;

	if (anim.m_cols == 0 || anim.m_framesStorage.empty()) {
		std::cerr << "[Animator] Error: m_cols == 0 or m_framesStorage empty for '" << name << "'.\n";
		return;
	}

	// ЛОГИКА «ПЕРЕЗАГРУЗКИ»: чистим только направления для ЭТОЙ анимации
	anim.m_directionalFrames.clear();

	const unsigned int cols = anim.m_cols;
	const auto& fullStorage = anim.m_framesStorage;
	constexpr int totalDirections = 8;

	for (int row = 0; row < totalDirections; ++row)
	{
		Direction dir = static_cast<Direction>(row);
		std::vector<sf::IntRect> frames;
		frames.reserve(cols);

		size_t startIndex = row * cols;
		for (unsigned int i = 0; i < cols; ++i)
		{
			if (startIndex + i < fullStorage.size())
				frames.push_back(fullStorage[startIndex + i]);
		}
		anim.m_directionalFrames[dir] = std::move(frames);
	}

	// !!! УБРАНО: Принудительная установка Down.
	// Мы просто берём текущее направление игрока (m_currentDirection)
	// и ставим ему соответствующий вектор кадров.

	auto dirIt = anim.m_directionalFrames.find(m_currentDirection);
	if (dirIt != anim.m_directionalFrames.end())
	{
		anim.m_frames = dirIt->second;
	}
	else
	{
		// Фоллбэк: если вдруг такого направления нет (редкий кейс), ставим Down
		auto downIt = anim.m_directionalFrames.find(Direction::Down);
		if (downIt != anim.m_directionalFrames.end())
			anim.m_frames = downIt->second;
	}

	// Сброс таймера и кадра при смене анимации
	m_elapsedTime = 0.0f;
	m_currentFrame = 0;
	m_endAnim = false;

	switchAnimation(&anim);
}



// Метод установки направления : теперь это просто переключение указателя / вектора
void Animator::setDirection(Direction dir)
{
	if (!m_currentAnimation) return;

	m_currentDirection = dir;

	auto it = m_currentAnimation->m_directionalFrames.find(dir);
	if (it != m_currentAnimation->m_directionalFrames.end())
	{
		m_currentAnimation->m_frames = it->second;

		// --- СБРОС ПРИ СМЕНЕ НАПРАВЛЕНИЯ ---
		m_elapsedTime = 0.0f;      // сбрасываем таймер, чтобы не прыгнуть вперёд
		m_currentFrame = 0;        // начинаем с первого кадра направления
		m_endAnim = false;        // говорим, что анимация ещё не закончилась
		// ----------------------------------
	}
}


void Animator::switchAnimation(Animation* animation) {
	if (!animation) {
		m_currentAnimation = nullptr;
		m_endAnim = true;
		return;
	}

	m_currentAnimation = animation;
	// Сбрасываем кадр при переключении анимации — чтобы анимация начиналась заново
	m_elapsedTime = 0.0f;
	m_currentFrame = 0;
	m_endAnim = false;
}

bool Animator::switchAnimation(std::string const& name) {
	auto it = m_animations.find(name);
	if (it == m_animations.end()) {
		return false;
	}
	switchAnimation(&it->second);
	return true;
}

void Animator::update(sf::Time const& dT) {
	if (!m_currentAnimation || m_stopAnimation) {
		m_endAnim = true;
		return;
	}

	const auto num_frames = static_cast<int>(m_currentAnimation->m_frames.size());
	if (num_frames == 0) {
		m_endAnim = true;
		return;
	}

	m_elapsedTime += dT.asSeconds();

	float framesPerSecond = m_currentAnimation->m_fps;
	if (framesPerSecond <= 0.0f) framesPerSecond = 12.0f;

	const float timePerFrame = 1.0f / framesPerSecond;

	// ВАЖНО: меняем только ОДИН кадр за один тик, даже если времени прошло много.
	// Это убирает «пропуск» кадров и мгновенный прыжок к концу.
	if (m_elapsedTime >= timePerFrame) {
		m_elapsedTime -= timePerFrame;
		++m_currentFrame;

		// Если анимация зациклена — просто делаем кольцо
		if (m_currentAnimation->m_looping) {
			if (m_currentFrame >= num_frames) {
				m_currentFrame = 0;
			}
		}
		else {
			// Для НЕ зацикленной анимации: как только мы вышли за пределы кадров —
			// останавливаемся на последнем и ставим флаг конца.
			if (m_currentFrame >= num_frames) {
				m_currentFrame = num_frames - 1;
				m_endAnim = true;
			}
			else {
				m_endAnim = false; // Анимация ещё идёт
			}
		}
	}

	// Отладочный вывод (оставляем как договаривались)
	std::cout << "Anim: " << m_currentAnimation->m_name
		<< " | Frame: " << m_currentFrame
		<< (m_endAnim && !m_currentAnimation->m_looping ? "\n" : "\r");

	m_sprite.setTextureRect(m_currentAnimation->m_frames[m_currentFrame]);
}




std::string Animator::getCurrentAnimationName() const {
	return m_currentAnimation ? m_currentAnimation->m_name : "";
}

void Animator::restart() {
	m_elapsedTime = 0.0f;
	m_currentFrame = 0;
	m_endAnim = false;
}

bool Animator::getEndAnim() const {
	return m_endAnim;
}

Animator::Animation* Animator::getCurrentAnimation() {
	return m_currentAnimation;
}

int Animator::getCurrentFrame() const {
	if (!m_currentAnimation) return 0;
	const auto numFrames = static_cast<int>(m_currentAnimation->m_frames.size());
	return (numFrames == 0) ? 0 : static_cast<int>(m_currentFrame);
}


