#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>

enum class Music {
	MenuTheme,
	TownTheme,
	MissionTheme
};

class MusicPlayer : private sf::NonCopyable {
public:
	MusicPlayer();

	void play(Music theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

private:
	sf::Music m_music;
	std::map<Music, std::string> m_filenames;
	float m_volume = 100.f;
};
