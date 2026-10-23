#include "main/asset_manager.hpp"
#include <stdexcept>
#include <iostream>

// ----------------------------------------------------------------------------
// Реализация синглтона
// ----------------------------------------------------------------------------

AssetManager& AssetManager::getInstance()
{
	// Статическая локальная переменная:
	// 1. Создаётся ровно один раз при первом вызове функции.
	// 2. Потокобезопасна (C++11+).
	// 3. Удаляется автоматически при завершении программы.
	static AssetManager instance;
	return instance;
}

// ----------------------------------------------------------------------------
// Работа с текстурами
// ----------------------------------------------------------------------------

sf::Texture& AssetManager::getTexture(std::string const& filename)
{
	// Получаем ссылку на текущий экземпляр синглтона
	auto& instance = getInstance();
	auto& textureMap = instance.m_textures;

	auto it = textureMap.find(filename);
	if (it != textureMap.end()) {
		return it->second;
	}

	// Если не найдено — создаем новую запись в мапе и загружаем
	auto& texture = textureMap[filename];
	if (!texture.loadFromFile(filename)) {
		throw std::runtime_error("AssetManager::getTexture - Failed to load " + filename);
	}
	return texture;
}

// ----------------------------------------------------------------------------
// Подготовка кадров атласа
// ----------------------------------------------------------------------------

std::vector<sf::IntRect>& AssetManager::prepareAtlasFrames(std::string const& textureName,
	sf::Vector2i const& frameSize,
	unsigned int columns,
	unsigned int rows)
{
	// Сначала убедимся, что текстура загружена (это также проверит существование файла)
	getTexture(textureName);

	auto& instance = getInstance();
	auto& framesMap = instance.m_atlasFrames;

	// Ссылка на вектор кадров для этой текстуры
	auto& frames = framesMap[textureName];

	frames.clear();
	frames.reserve(columns * rows);

	// ВСЕГДА начинаем с (0, 0). Никаких смещений.
	const int startX = 0;
	const int startY = 0;

	for (unsigned int row = 0; row < rows; ++row) {
		for (unsigned int col = 0; col < columns; ++col) {
			int x = startX + (col * frameSize.x);
			int y = startY + (row * frameSize.y);
			frames.emplace_back(x, y, frameSize.x, frameSize.y);
		}
	}

	return frames;
}

// ----------------------------------------------------------------------------
// Получение готовых кадров
// ----------------------------------------------------------------------------

const std::vector<sf::IntRect>* AssetManager::getAtlasFrames(std::string const& textureName)
{
	auto& instance = getInstance();
	const auto& framesMap = instance.m_atlasFrames;

	auto it = framesMap.find(textureName);
	if (it == framesMap.end()) {
		return nullptr;
	}
	return &it->second;
}

// ----------------------------------------------------------------------------
// Работа со звуковыми буферами
// ----------------------------------------------------------------------------

sf::SoundBuffer& AssetManager::getSoundBuffer(std::string const& filename)
{
	auto& instance = getInstance();
	auto& sBufferMap = instance.m_soundBuffers;

	auto it = sBufferMap.find(filename);
	if (it != sBufferMap.end()) {
		return it->second;
	}

	auto& buffer = sBufferMap[filename];
	if (!buffer.loadFromFile(filename)) {
		throw std::runtime_error("AssetManager::getSoundBuffer - Failed to load " + filename);
	}
	return buffer;
}

// ----------------------------------------------------------------------------
// Работа со шрифтами
// ----------------------------------------------------------------------------

sf::Font& AssetManager::getFont(std::string const& filename)
{
	auto& instance = getInstance();
	auto& fontMap = instance.m_fonts;

	auto it = fontMap.find(filename);
	if (it != fontMap.end()) {
		return it->second;
	}

	auto& font = fontMap[filename];
	if (!font.loadFromFile(filename)) {
		throw std::runtime_error("AssetManager::getFont - Failed to load " + filename);
	}
	return font;
}




