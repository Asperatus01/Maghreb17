#pragma once

#include <SFML/Graphics.hpp>
#include "shared_context.hpp"
#include <string>
#include <functional>
#include <unordered_map>

// Локальный заглушечный enum, чтобы проект собрался без подключения State.h
enum class StateType {
	None = 0,
	InGame = 1,
	Menu = 2
};

struct EventDetails {
	std::string m_name;
	sf::Vector2i m_size{ 0, 0 };
	sf::Uint32 m_textEntered{ 0 };
	sf::Vector2i m_mouse{ 0, 0 };
	int m_mouseWheelDelta{ 0 };
	int m_keyCode{ -1 };

	void Clear() {
		m_size = { 0, 0 };
		m_textEntered = 0;
		m_mouse = { 0, 0 };
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}
};

class EventManager {
public:
	explicit EventManager(SharedContext& context);
	~EventManager();

	// Эти методы оставляем для совместимости с кодом, но делаем их пустыми
	bool AddBinding(void* /*binding*/) { return false; } // Приводим к void*, чтобы не зависеть от структуры Binding
	bool RemoveBinding(const std::string& name) { return false; }

	void SetCurrentState(StateType state);
	void SetFocus(bool focus);

	// Заглушка для шаблона AddCallback: сигнатура должна быть, но реализация упрощена.
	// В реальной реализации тут нужен std::bind, но для компиляции нам хватит просто объявления.
	template<class T>
	bool AddCallback(StateType, const std::string&, void(T::*)(EventDetails*), T*) {
		return true; // Просто говорим "ок", чтобы код не падал на проверке возврата
	}

	bool RemoveCallback(StateType state, const std::string& name);

	void HandleEvent(sf::Event& event);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* wind = nullptr);

private:
	SharedContext& m_context;

	// Упрощённые контейнеры для заглушки (можно было бы вообще убрать, 
	// но оставим, чтобы методы Update/HandleEvent имели тело)
	std::unordered_map<std::string, std::function<void(EventDetails*)>> m_callbacks;
	StateType m_currentState;
	bool m_hasFocus;
};

