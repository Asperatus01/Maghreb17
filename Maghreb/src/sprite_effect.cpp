#include "objects/sprite_effect.hpp"
#include <iostream>

void SpriteEffect::createStrokedSprite(
	sf::Sprite& m_strokeSprite,
	sf::Texture& m_strokeTexture,
	const sf::Sprite& m_lastFrame
) {
	// Получаем из спрайта текстурный прямоугольник
	sf::IntRect textureRect = m_lastFrame.getTextureRect();

	// Если текстура не установлена или прямоугольник нулевой — выходим, чтобы не было краша
	if (!m_lastFrame.getTexture() || textureRect.width <= 0 || textureRect.height <= 0) {
		return;
	}

	// Получаем из спрайта изображение (копируем всю текстуру)
	sf::Image img = m_lastFrame.getTexture()->copyToImage();

	// Создаём изображение размером с текстурный прямоугольник и делаем его прозрачным
	sf::Image strokedImage;
	strokedImage.create(textureRect.width, textureRect.height, sf::Color::Transparent);

	// Копируем в него нужный фрагмент из полной картинки
	strokedImage.copy(img, 0, 0, textureRect);

	// Отдельный слой только для обводки — он изначально прозрачный
	sf::Image outlineImage;
	outlineImage.create(textureRect.width, textureRect.height, sf::Color::Transparent);

	bool solidPixel;

	// --- Сканирование по строкам (горизонтальные границы) ---
	for (unsigned int i = 0; i < strokedImage.getSize().y; ++i) {
		solidPixel = false; // Сбрасываем флаг для каждой новой строки

		for (unsigned int j = 0; j < strokedImage.getSize().x; ++j) {
			if (!solidPixel && isPixelSolid(strokedImage, j, i)) {
				// Нашли начало сплошной области — рисуем обводку слева от неё
				if (j > 0) {
					outlineImage.setPixel(j - 1, i, sf::Color(0, 255, 255));
				}
				solidPixel = true;
			}
			else if (solidPixel && !isPixelSolid(strokedImage, j, i)) {
				// Конец сплошной области — рисуем обводку справа от неё
				outlineImage.setPixel(j, i, sf::Color(0, 255, 255));
				solidPixel = false;
			}
		}
	}

	// --- Сканирование по столбцам (вертикальные границы) ---
	for (unsigned int i = 0; i < strokedImage.getSize().x; ++i) {
		solidPixel = false; // Сбрасываем флаг для каждого нового столбца

		for (unsigned int j = 0; j < strokedImage.getSize().y; ++j) {
			if (!solidPixel && isPixelSolid(strokedImage, i, j)) {
				// Начало сплошной области — рисуем обводку сверху
				if (j > 0) {
					outlineImage.setPixel(i, j - 1, sf::Color(0, 255, 255));
				}
				solidPixel = true;
			}
			else if (solidPixel && !isPixelSolid(strokedImage, i, j)) {
				// Конец сплошной области — рисуем обводку снизу
				outlineImage.setPixel(i, j, sf::Color(0, 255, 255));
				solidPixel = false;
			}
		}
	}

	// --- Накладываем обводку поверх исходного кадра (попиксельно) ---
	// Проходим по всем пикселям outlineImage и копируем только не-прозрачные
	const unsigned int w = outlineImage.getSize().x;
	const unsigned int h = outlineImage.getSize().y;
	for (unsigned int y = 0; y < h; ++y) {
		for (unsigned int x = 0; x < w; ++x) {
			sf::Color c = outlineImage.getPixel(x, y);
			// Если пиксель не прозрачный — копируем его поверх кадра
			if (c.a != 0) {
				strokedImage.setPixel(x, y, c);
			}
		}
	}

	// Создаём текстуру нужного размера
	m_strokeTexture.create(strokedImage.getSize().x, strokedImage.getSize().y);

	// Обновляем текстуру данными из изображения
	m_strokeTexture.update(strokedImage);

	// Привязываем текстуру к выходному спрайту (true = не копировать текстуру внутрь спрайта)
	m_strokeSprite.setTexture(m_strokeTexture, true);
}

void SpriteEffect::createRedSprite(sf::Sprite& m_redSprite, sf::Texture& m_redTexture, const sf::Texture& m_primaryTexture) {
	// Проверка на валидность текстуры: если ширина или высота 0 — текстура не создана
	if (m_primaryTexture.getSize().x == 0 || m_primaryTexture.getSize().y == 0) {
		return;
	}

	// Получаем из текстуры изображение
	sf::Image redImage = m_primaryTexture.copyToImage();

	// Получаем ширину и высоту
	unsigned int width = redImage.getSize().x;
	unsigned int height = redImage.getSize().y;

	// Если размеры нулевые — ничего не делаем
	if (width == 0 || height == 0) {
		return;
	}

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			sf::Color color = redImage.getPixel(x, y);

			// Обнуляем зелёный и синий каналы, оставляем R и A как есть
			color.g = 0;
			color.b = 0;

			redImage.setPixel(x, y, color);
		}
	}

	// Создаём текстуру нужного размера
	m_redTexture.create(width, height);

	// Обновляем текстуру данными из изображения
	m_redTexture.update(redImage);

	// Привязываем текстуру к спрайту (true = не копировать текстуру внутрь спрайта)
	m_redSprite.setTexture(m_redTexture, true);
}

bool SpriteEffect::isPixelSolid(const sf::Image& img, unsigned int x, unsigned int y) {
	return img.getPixel(x, y).a != 0;
}
