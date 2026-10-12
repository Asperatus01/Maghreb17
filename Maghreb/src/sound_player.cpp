#include "sound/sound_player.hpp"
#include <algorithm>
#include <iostream>

// Forward declaration: этот класс должен быть реализован отдельно
class AssetManager;

static const sf::SoundBuffer* getSoundBufferStub(const std::string&) {
	static sf::SoundBuffer dummy;
	return &dummy;
}

const sf::SoundBuffer* getAssetBuffer(const std::string& path) {
	// TODO: заменить на реальный вызов AssetManager::GetSoundBuffer(path)
	// return AssetManager::GetSoundBuffer(path);
	return getSoundBufferStub(path);
}

SoundPlayer::SoundPlayer() {
	m_soundBuffers[SoundEffect::walk1] = getAssetBuffer("Resources/Sound/player/walk1.wav");
	m_soundBuffers[SoundEffect::bfire] = getAssetBuffer("Resources/Sound/player/bfire.wav");
	m_soundBuffers[SoundEffect::damage_player] = getAssetBuffer("Resources/Sound/player/damage_player.wav");
	m_soundBuffers[SoundEffect::bhit1] = getAssetBuffer("Resources/Sound/player/bhit1.wav");
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
