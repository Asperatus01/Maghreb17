#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "ai/direction.hpp"

class Animator {
public:
	struct Animation {
		std::string m_name;
		unsigned int m_cols = 0;                  // кадров в строке (столбцов)
		unsigned int m_rows = 0;                  // строк (направлений)
		sf::Vector2i m_frameSize{ 0, 0 };         // размер одного кадра
		std::vector<sf::IntRect> m_frames;        // кадры для отрисовки (текущее направление)
		std::vector<sf::IntRect> m_framesStorage; // полный атлас всех кадров
		float m_fps = 12.0f;                      // кадров в секунду
		bool m_looping = true;                    // флаг зацикливания

		// Направления хранятся ВНУТРИ анимации: ключ — направление, значение — вектор кадров
		std::unordered_map<Direction, std::vector<sf::IntRect>> m_directionalFrames;

		Animation() = default;

		Animation(std::string const& name, unsigned int cols, unsigned int rows, float fps, bool looping)
			: m_name(name)
			, m_cols(cols)
			, m_rows(rows)
			, m_fps(fps)
			, m_looping(looping)
		{}
	};


	explicit Animator(sf::Sprite& sprite);

	Animation& createAnimation(std::string const& name,
		unsigned int cols,
		unsigned int rows,
		float fps,
		bool looping);


	// Метод загрузки, который сразу нарезает все направления
	void loadDirectionalAnimation(const std::string& name);

	// Просто скажи, куда смотришь
	void setDirection(Direction dir);

	Direction getCurrentDirection() const noexcept {
		return m_currentDirection;
	}

	// Получить список всех загруженных анимаций
	std::vector<std::string> getAnimationNames() const {
		std::vector<std::string> names;
		names.reserve(m_animations.size());
		for (const auto& it : m_animations) {
			names.push_back(it.first); // это ключ (имя анимации)
		}
		return names;
	}


	void update(sf::Time const& deltaTime);
	bool switchAnimation(std::string const& name);
	std::string getCurrentAnimationName() const;
	void restart();
	bool getEndAnim() const;
	void resetEndAnim() { m_endAnim = true; }; // Сброс конца анимации для однократного проигрывателя звука

	// Внутренние
	void switchAnimation(Animation* animation);
	Animation* getCurrentAnimation();
	int getCurrentFrame() const;

private:
	sf::Sprite& m_sprite;
	//sf::Time m_currentTime;  можно оставить, если он нужен отдельно, но не используй его для расчёта кадра

	float m_elapsedTime = 0.0f;     // вместо m_currentTime для расчёта кадров
	size_t m_currentFrame = 0;      // индекс текущего кадра

	// Карта с анимациями "обобщённое имя" : "структура Animation"
	std::unordered_map<std::string, Animation> m_animations;

	Direction m_currentDirection;

	Animation* m_currentAnimation;
	bool m_stopAnimation;
	bool m_endAnim;
};
