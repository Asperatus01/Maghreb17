#include "sound/sound_player.hpp"
#include "main/asset_manager.hpp"
#include <algorithm>
#include <iostream>

static const sf::SoundBuffer* getSoundBufferStub(const std::string&) {
	static sf::SoundBuffer dummy;
	return &dummy;
}

const sf::SoundBuffer* getAssetBuffer(const std::string& path) {
	static const sf::SoundBuffer& buffer = AssetManager::getSoundBuffer(path);
	return &buffer;
}


SoundPlayer::SoundPlayer() {
	m_soundBuffers[SoundEffect::walk1] = getAssetBuffer("res/sound/player/walk1.wav");
	m_soundBuffers[SoundEffect::bfire] = getAssetBuffer("res/sound/player/bfire.wav");
	m_soundBuffers[SoundEffect::damage_player] = getAssetBuffer("res/sound/player/damage_player.wav");
	m_soundBuffers[SoundEffect::bhit1] = getAssetBuffer("res/sound/player/bhit1.wav");
}

void SoundPlayer::play(SoundEffect effect) {
	auto it = m_soundBuffers.find(effect);
	if (it == m_soundBuffers.end() || !it->second) {
		return; // или лог ошибки
	}

	const sf::SoundBuffer* buffer = it->second;

	// »щем свободный Sound (не в состо€нии Playing)
	auto freeIt = std::find_if(m_playingSounds.begin(), m_playingSounds.end(),
		[](const sf::Sound& s) { return s.getStatus() != sf::Sound::Playing; });

	if (freeIt != m_playingSounds.end()) {
		freeIt->setBuffer(*buffer);
		freeIt->play();
	}
	else {
		// ѕул закончилс€ Ч добавл€ем новый
		m_playingSounds.emplace_back();
		m_playingSounds.back().setBuffer(*buffer);
		m_playingSounds.back().play();
	}
}

void SoundPlayer::play(const std::string& filename) {
	const sf::SoundBuffer* buffer = getAssetBuffer(filename);
	if (!buffer) {
		return;
	}

	auto freeIt = std::find_if(m_playingSounds.begin(), m_playingSounds.end(),
		[](const sf::Sound& s) { return s.getStatus() != sf::Sound::Playing; });

	if (freeIt != m_playingSounds.end()) {
		freeIt->setBuffer(*buffer);
		freeIt->play();
	}
	else {
		m_playingSounds.emplace_back();
		m_playingSounds.back().setBuffer(*buffer);
		m_playingSounds.back().play();
	}
}

void SoundPlayer::removeStoppedSounds() {
	auto newEnd = std::remove_if(m_playingSounds.begin(), m_playingSounds.end(),
		[](const sf::Sound& s) {
		return s.getStatus() == sf::Sound::Stopped;
	});
	m_playingSounds.erase(newEnd, m_playingSounds.end());
}
