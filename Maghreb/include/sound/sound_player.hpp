#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#include <string>

class AssetManager;

enum class SoundEffect {
	walk1,
	bfire,
	damage_player,
	bhit1
};

class SoundPlayer : private sf::NonCopyable {
public:
	SoundPlayer();

	void play(SoundEffect effect);
	void play(const std::string& filename);
	void removeStoppedSounds();

private:
	std::map<SoundEffect, const sf::SoundBuffer*> m_soundBuffers;
	std::vector<sf::Sound> m_playingSounds;
};

