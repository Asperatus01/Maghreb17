#pragma once

#include <SFML/Graphics.hpp>

// Подключаем все нужные части tmxlite
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>      // Обязательно: здесь определено FlipFlag
#include <tmxlite/TileLayer.hpp>  // Обязательно: здесь определён TileLayer

#include <vector>
#include <string>
#include <memory>

class Level
{
public:
	bool load(const std::string& path);
	void draw(sf::RenderTarget& target) const;

	// Геттер размера карты
	tmx::Vector2u getSize() const
	{
		return m_map.getTileCount();
	}
 
	sf::Vector2u getTileSize() const;   // размер тайла в пикселях
	sf::Vector2u getTileCount() const;  // размер карты в тайлах
private:
	struct LayerDraw
	{
		sf::Texture texture;
		sf::VertexArray vertices;
	};

	tmx::Map m_map;
	std::vector<std::unique_ptr<LayerDraw>> m_layers;
};

class TileManager
{
public:
	bool loadLevel(std::size_t index, const std::string& path);
	void activate(std::size_t index);
	tmx::Vector2u getSize() const;
	void draw(sf::RenderTarget& target) const;
	const Level& getCurrentLevel() const;
	

private:
	std::vector<std::unique_ptr<Level>> m_levels;
	std::size_t m_active = 0;
};
