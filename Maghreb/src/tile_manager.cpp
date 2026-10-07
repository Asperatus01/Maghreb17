#include <iostream>
#include <algorithm>
#include <filesystem>
#include <tmxlite/Layer.hpp> 
#include "loader/tile_manager.hpp"


bool Level::load(const std::string& path)
{
	if (!m_map.load(path))
	{
		std::cerr << "Не удалось загрузить карту: " << path << "\n";
		return false;
	}

	const std::filesystem::path baseDir = std::filesystem::path(path).parent_path();

	auto resolveImagePath = [&](const std::string& rawPath) -> std::filesystem::path
	{
		const std::filesystem::path candidate = baseDir / rawPath;
		if (std::filesystem::exists(candidate))
			return candidate;
		return baseDir / std::filesystem::path(rawPath).filename();
	};

	const auto& tilesets = m_map.getTilesets();
	const auto mapSize = m_map.getTileCount();
	const auto tileSize = m_map.getTileSize();

	for (const auto& layerPtr : m_map.getLayers())
	{
		if (layerPtr->getType() != tmx::Layer::Type::Tile)
			continue;

		const auto& tileLayer = layerPtr->getLayerAs<tmx::TileLayer>();
		const auto& tiles = tileLayer.getTiles();

		for (const auto& ts : tilesets)
		{
			auto layerDraw = std::make_unique<LayerDraw>();

			const std::filesystem::path imagePath = resolveImagePath(ts.getImagePath());
			if (!layerDraw->texture.loadFromFile(imagePath.string()))
			{
				std::cerr << "Не удалось загрузить текстуру тайлсета: "
					<< imagePath.string() << "\n";
				return false;
			}
	
		
			layerDraw->vertices.setPrimitiveType(sf::Quads);

			const auto tsSize = ts.getTileSize();
			const auto columns = ts.getColumnCount();

            for (std::size_t i = 0; i < tiles.size(); ++i)
			{
				const auto& tile = tiles[i];

				if (tile.ID == 0 ||
					tile.ID < ts.getFirstGID() ||
					tile.ID >= ts.getFirstGID() + ts.getTileCount())
				{
					continue;
				}

				const std::uint32_t tileIndex = tile.ID - ts.getFirstGID();
				const float tx = static_cast<float>((tileIndex % columns) * tsSize.x);
				const float ty = static_cast<float>((tileIndex / columns) * tsSize.y);

				const float x = static_cast<float>((i % mapSize.x) * tileSize.x);
				const float y = static_cast<float>((i / mapSize.x) * tileSize.y);

				// Пиксельные координаты (SFML сам нормализует)
				float u0 = tx;
				float v0 = ty;
				float u1 = tx + tsSize.x;
				float v1 = ty + tsSize.y;

				const bool flipX = (tile.flipFlags & tmx::TileLayer::FlipFlag::Horizontal) != 0;
				const bool flipY = (tile.flipFlags & tmx::TileLayer::FlipFlag::Vertical) != 0;
				if (flipX) std::swap(u0, u1);
				if (flipY) std::swap(v0, v1);

				sf::Vertex quad[4];
				quad[0].position = sf::Vector2f(x, y);
				quad[0].texCoords = sf::Vector2f(u0, v0);
				quad[1].position = sf::Vector2f(x + tileSize.x, y);
				quad[1].texCoords = sf::Vector2f(u1, v0);
				quad[2].position = sf::Vector2f(x + tileSize.x, y + tileSize.y);
				quad[2].texCoords = sf::Vector2f(u1, v1);
				quad[3].position = sf::Vector2f(x, y + tileSize.y);
				quad[3].texCoords = sf::Vector2f(u0, v1);

				layerDraw->vertices.append(quad[0]);
				layerDraw->vertices.append(quad[1]);
				layerDraw->vertices.append(quad[2]);
				layerDraw->vertices.append(quad[3]);
			}

			m_layers.push_back(std::move(layerDraw));
		}
	}

	return true;
}



void Level::draw(sf::RenderTarget& target) const
{
	for (const auto& layerDraw : m_layers)
	{
		sf::RenderStates states;
		states.texture = &layerDraw->texture;
		target.draw(layerDraw->vertices, states);
	}
}

//void Level::draw(sf::RenderTarget& target) const
//{
//	for (const auto& layerDraw : m_layers)
//	{
//		const auto texSize = layerDraw->texture.getSize();
//		const std::size_t count = layerDraw->vertices.getVertexCount();
//
//		std::cout << "[DEBUG] vertices: " << count
//			<< ", texture: " << texSize.x << " x " << texSize.y << "\n";
//
//		// Первые два квада целиком: позиция + texCoords всех 4 вершин
//		for (std::size_t v = 0; v < count && v < 8; ++v)
//		{
//			const auto& vert = layerDraw->vertices[v];
//			std::cout << "  v" << v << ": pos(" << vert.position.x << ", " << vert.position.y
//				<< ") tex(" << vert.texCoords.x << ", " << vert.texCoords.y << ")\n";
//		}
//
//		sf::RenderStates states;
//		states.texture = &layerDraw->texture;
//		target.draw(layerDraw->vertices, states);
//	}
//}


sf::Vector2u Level::getTileSize() const
{
	// m_map — это твой tmx::Map из tmxlite. Если поле называется иначе — подставь имя.
	const auto ts = m_map.getTileSize();
	return sf::Vector2u(static_cast<unsigned int>(ts.x),
		static_cast<unsigned int>(ts.y));
}

sf::Vector2u Level::getTileCount() const
{
	const auto tc = m_map.getTileCount();
	return sf::Vector2u(static_cast<unsigned int>(tc.x),
		static_cast<unsigned int>(tc.y));
}



bool TileManager::loadLevel(std::size_t index, const std::string& path)
{
	if (m_levels.size() <= index)
	{
		m_levels.resize(index + 1);
	}

	auto level = std::make_unique<Level>();
	if (!level->load(path))
	{
		return false;
	}

	m_levels[index] = std::move(level);
	std::cout << "Уровень " << index << " загружен.\n";
	return true;
}

void TileManager::activate(std::size_t index)
{
	if (index < m_levels.size() && m_levels[index])
	{
		m_active = index;
	}
}

tmx::Vector2u TileManager::getSize() const
{
	if (m_active < m_levels.size() && m_levels[m_active])
	{
		// Используем публичный геттер у Level вместо прямого доступа к m_map
		return m_levels[m_active]->getSize();
	}
	return tmx::Vector2u{ 0, 0 };
}

const Level& TileManager::getCurrentLevel() const
{
	if (m_active >= m_levels.size())
		throw std::out_of_range("Нет активного уровня");

	// m_levels[m_active] — это unique_ptr<Level>
	// *m_levels[...] — разыменовывает указатель и даёт ссылку на Level
	return *m_levels[m_active];
}


void TileManager::draw(sf::RenderTarget& target) const
{
	if (m_active < m_levels.size() && m_levels[m_active])
	{
		m_levels[m_active]->draw(target);
	}
}
