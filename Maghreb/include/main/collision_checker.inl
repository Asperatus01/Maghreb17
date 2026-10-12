// Заглушки для шаблонных методов — пустые тела,
// чтобы код компилировался без TileManager и Entity

template<typename T>
void CollisionChecker::findTile(T& /*entity*/, int /*row*/, int /*col*/) {
	// Реальная логика проверки тайла вернётся позже,
	// когда TileManager будет реализован
}

template<typename T>
void CollisionChecker::checkTile(T& /*entity*/) {
	// Реальная логика проверки коллизий с тайлами вернётся позже
}

