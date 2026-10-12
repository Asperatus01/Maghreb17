#include "sound/music_player.hpp"
#include <stdexcept>
#include <iostream>

MusicPlayer::MusicPlayer() {
	m_filenames[Music::MenuTheme] = "Resources/Sound/music/MenuTheme.wav";
	m_filenames[Music::TownTheme] = "Resources/Sound/music/TownTheme.wav";
	m_filenames[Music::MissionTheme] = "Resources/Sound/music/MissionTheme.wav";
}

void MusicPlayer::play(Music theme) {
	auto it = m_filenames.find(theme);
	if (it == m_filenames.end()) {
		throw std::runtime_error("Unknown music theme");
	}

	const std::string& filename = it->second;

	if (!m_music.openFromFile(filename)) {
		std::cerr << "[Error] Music could not be loaded: " << filename << '\n';
		throw std::runtime_error("Music " + filename + " could not be loaded.");
	}

	m_music.setVolume(m_volume);
	m_music.setLoop(true);
	m_music.play();
}

void MusicPlayer::stop() {
	m_music.stop();
}

void MusicPlayer::setVolume(float volume) {
	// SFML volume: 0–100 (или больше, но 100 — максимум по смыслу)
	m_volume = volume;
	m_music.setVolume(m_volume);
}

void MusicPlayer::setPaused(bool paused) {
	if (paused) {
		m_music.pause();
	}
	else {
		// Если музыка уже играет, play() ничего не сделает, это безопасно
		m_music.play();
	}
}
