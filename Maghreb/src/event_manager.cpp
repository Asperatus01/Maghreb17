#include "main/event_manager.hpp"
#include <iostream>

EventManager::EventManager(SharedContext& context)
	: m_context(context)
	, m_currentState(StateType::None)
	, m_hasFocus(true)
{
	std::cerr << "[Stub] EventManager created\n";
}

EventManager::~EventManager() {
	std::cerr << "[Stub] EventManager destroyed\n";
	// Никаких циклов удаления Binding*, так как мы их не создавали
}

void EventManager::SetCurrentState(StateType state) {
	m_currentState = state;
}

void EventManager::SetFocus(bool focus) {
	m_hasFocus = focus;
}

bool EventManager::RemoveCallback(StateType, const std::string&) {
	return false; // Заглушка
}

void EventManager::HandleEvent(sf::Event& event) {
	// В заглушке просто логируем, что событие пришло
	if (event.type == sf::Event::Closed) {
		// Можно было бы тут дернуть контекст, но пока просто игнорируем
	}
	else if (event.type == sf::Event::KeyPressed) {
		// Логика обработки будет позже
	}
	// Реальная логика биндингов отключена, чтобы не требовать keys.cfg
}

void EventManager::Update() {
	if (!m_hasFocus) {
		return;
	}
	// Здесь была сложная логика проверки count и вызова колбэков.
	// Для заглушки оставляем пустым: это позволит игре компилироваться и крутиться.
	// Когда дойдем до реализации, сюда вернется логика.
}

sf::Vector2i EventManager::GetMousePos(sf::RenderWindow* wind) {
	if (wind) {
		return sf::Mouse::getPosition(*wind);
	}
	return sf::Mouse::getPosition();
}
