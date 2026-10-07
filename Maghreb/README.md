# Maghreb17

ѕроект 2D-игры на C++20 + SFML.  арта генерируетс€ в Tiled, парситс€ через tmxlite.

## —труктура проекта

- `assets/levels/` Ч карты уровней (.tmx) и атласы текстур (.png).
- `include/` Ч публичные заголовки.
  - `loader/tile_manager.hpp` Ч загрузка и отрисовка тайлов.
  - `main/game.hpp` Ч основной класс игры и игровой цикл.
- `libs/tmxlite/` Ч сторонн€€ библиотека дл€ парсинга TMX.
  - `include/` Ч заголовки tmxlite.
  - `src/` Ч исходники tmxlite (компилируютс€ вместе с проектом).
- `src/` Ч реализаци€ проекта.
  - `game.cpp` Ч логика игрового цикла.
  - `tile_manager.cpp` Ч отрисовка карты.
  - `main/` Ч точка входа (`main.cpp`).

## —борка

ѕроект собираетс€ в Visual Studio. ”бедитесь, что пути к заголовочным файлам (`Additional Include Directories`) настроены на папки `include` и `libs/tmxlite/include/tmxlite`.
