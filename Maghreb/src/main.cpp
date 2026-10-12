#include <iostream>
#include <memory>
#include "main/shared_context.hpp"
#include "main/game.hpp"

int main()
{
	// Устанавливаем локаль, чтобы в консоли нормально выводились русские сообщения
#ifdef _WIN32
	system("chcp 65001 > nul");
#endif
	// 1. Сначала создаём контекст
	SharedContext context;

	try
	{
		// Создаём игру через std::make_unique — это современный стиль C++20
		auto game = std::make_unique<Game>(context);

		// Запускаем игровой цикл
		game->run();
	}
	catch (const std::exception& e)
	{
		// Если что-то пошло не так (например, не загрузился уровень),
		// выводим понятное сообщение и завершаем программу с кодом ошибки
		std::cerr << "Ошибка запуска игры: " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "Неизвестная ошибка запуска игры.\n";
		return 1;
	}

	return 0;
}
