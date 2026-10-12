#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

class Animator {
public:
	Animator() = default;
	~Animator() = default;

	void loadAnimation(const std::string& textureName,
		unsigned int frameCol, unsigned int frameRow = 1);

	void update(sf::Time dt);
	void draw(sf::RenderTarget& target, const sf::Vector2f& position);

	void switchAnimation(const std::string& name);
	std::string getCurrentAnimationName() const;

	unsigned int getCurrentFrame() const;
	sf::Vector2u getFrameSize() const;

	bool isStopped() const;
	void setStopped(bool stopped);


	std::optional<size_t> findAnimationIndex(const std::string& name) const;

private:
	struct Animation {
		std::string m_name;
		std::vector<sf::Sprite> m_frames;
		unsigned int m_frameCol = 0;
		unsigned int m_frameRow = 0;
		float m_frameDuration;
		sf::Vector2u m_frameSize{ 0, 0 };

	};

	std::vector<Animation> m_animations;
	size_t m_currentAnim = 0;
	unsigned int m_currentFrame = 0;
	float m_elapsed = 0.f;
	bool m_stopAnimation = false;
};

