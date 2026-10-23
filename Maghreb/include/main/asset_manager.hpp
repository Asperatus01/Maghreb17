#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class AssetManager
{
public:
	// Единственный способ получить доступ к менеджеру.
	// Создаётся лениво (при первом вызове) и живёт до конца программы.
	static AssetManager& getInstance();

	// Методы для работы с ресурсами (остались без изменений по сигнатуре)
	static sf::Texture& getTexture(std::string const& filename);
	static sf::SoundBuffer& getSoundBuffer(std::string const& filename);
	static sf::Font& getFont(std::string const& filename);

	// Подготовка кадров для атласа анимации.
   // textureName — ключ (имя файла).
   // frameSize — размер одного кадра.
   // columns, rows — количество столбцов и строк в атласе.
   // Теперь всегда начинаем с (0, 0) и идём до конца.
	static std::vector<sf::IntRect>& prepareAtlasFrames(std::string const& textureName,
		sf::Vector2i const& frameSize,
		unsigned int columns,
		unsigned int rows);

	// Получить уже подготовленные кадры (для быстрого доступа из Animator)
	static const std::vector<sf::IntRect>* getAtlasFrames(std::string const& textureName);

private:
	// Приватный конструктор: нельзя создать объект вручную извне.
	AssetManager() = default;
	~AssetManager() = default;

	// Хранилища ресурсов (были m_textures и т.д., теперь они члены экземпляра)
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, std::vector<sf::IntRect>> m_atlasFrames; // храним кадры по имени текстуры
	std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
	std::unordered_map<std::string, sf::Font> m_fonts;
};

