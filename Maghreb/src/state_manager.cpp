#include "states/state_manager.hpp"
#include <iostream>

StateManager::StateManager(SharedContext& context)
	: m_context(context)
{
	std::cerr << "[Stub] StateManager created\n";

	// Попытка загрузить курсор, но без паники, если файла нет
	if (m_cursorImage.loadFromFile("Resources/Cursor/cursorfinger.png")) {
		const std::uint8_t* pixels = m_cursorImage.getPixelsPtr();
		m_cursor.loadFromPixels(pixels, m_cursorImage.getSize(), sf::Vector2u{ 0, 0 });
		m_cursorLoaded = true;
		std::cerr << "[Stub] Cursor loaded\n";
	}
	else {
		std::cerr << "[Stub] Could not load cursor (file missing or path wrong). Using default.\n";
		// Курсор останется дефолтным, игра не упадёт
	}
}

StateManager::~StateManager() {
	std::cerr << "[Stub] StateManager destroyed\n";
	// В заглушке НЕ делаем цикл удаления BaseState*, потому что:
	// 1. У нас нет реальных указателей на состояния (они nullptr или фиктивные)
	// 2. В новой архитектуре память будет управляться иначе (например, unique_ptr)
	for (auto& pair : m_states) {
		if (pair.second) {
			// Если вдруг кто-то положил туда указатель, удаляем аккуратно
			pair.second->OnDestroy();
			delete pair.second;
		}
	}
}

void StateManager::Update(const sf::Time& dT) {
	if (m_states.empty()) {
		return;
	}
	// Заглушка: обновляем только верхнее состояние
	m_states.back().second->Update(dT);
}

void StateManager::Draw() {
	// Получаем окно безопасно
	auto windowPtr = m_context.m_windPtr.get();
	if (!windowPtr) {
		return;
	}
	sf::RenderWindow* window = windowPtr->getRenderWindow(); // предполагаем, что метод переименован в getRenderWindow()

	// Убираем зависимость от Inventory/ControlPanel в заглушке
	window->setMouseCursor(m_cursor);

	if (m_states.empty()) {
		return;
	}

	// Рисуем только верхнее состояние (упрощённая логика вместо сложной стековой отрисовки)
	m_states.back().second->Draw();
}

SharedContext& StateManager::GetContext() {
	return m_context;
}

bool StateManager::HasState(const StateType& type) const {
	for (const auto& pair : m_states) {
		if (pair.first == type) {
			auto it = std::find(m_toRemove.begin(), m_toRemove.end(), type);
			return (it == m_toRemove.end());
		}
	}
	return false;
}

void StateManager::ProcessRequests() {
	while (!m_toRemove.empty()) {
		RemoveState(m_toRemove.front());
		m_toRemove.erase(m_toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& type) {
	// Синхронизируем с EventManager, если он есть
	if (m_context.m_eventManagerPtr) {
		m_context.m_eventManagerPtr->SetCurrentState(type);
	}

	// Ищем состояние в стеке
	for (auto it = m_states.begin(); it != m_states.end(); ++it) {
		if (it->first == type) {
			if (!m_states.empty() && m_states.back().first != type) {
				m_states.back().second->Deactivate();
			}
			// Перемещаем найденное состояние наверх
			BaseState* state = it->second;
			StateType stType = it->first;
			m_states.erase(it);
			m_states.emplace_back(stType, state);
			state->Activate();
			return;
		}
	}

	// Если состояние не найдено, пытаемся создать (в заглушке CreateState ничего не сделает, если нет фабрики)
	if (!m_states.empty()) {
		m_states.back().second->Deactivate();
	}
	CreateState(type);
	if (!m_states.empty()) {
		m_states.back().second->Activate();
	}
}

void StateManager::Remove(const StateType& type) {
	m_toRemove.push_back(type);
}

void StateManager::CreateState(const StateType& type) {
	auto it = m_stateFactory.find(type);
	if (it == m_stateFactory.end()) {
		std::cerr << "[Stub] No factory registered for state type: "
			<< static_cast<int>(type) << "\n";
		return;
	}
	BaseState* state = it->second();
	if (!state) {
		return;
	}

	// Инициализация вида (безопасно, если окно есть)
	auto windowPtr = m_context.m_windPtr.get();
	if (windowPtr) {
		state->m_view = windowPtr->getRenderWindow()->getDefaultView();
	}

	m_states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type) {
	for (auto it = m_states.begin(); it != m_states.end(); ++it) {
		if (it->first == type) {
			it->second->OnDestroy();
			delete it->second;
			m_states.
