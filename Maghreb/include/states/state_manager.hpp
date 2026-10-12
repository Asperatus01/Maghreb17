#pragma once

#include "main/shared_context.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

// Локальный заглушечный enum, чтобы не зависеть от State.h
enum class StateType {
	None = 0,
	Intro = 1,
	MainMenu = 2,
	Game = 3,
	Paused = 4,
	GameOver = 5,
	Credits = 6,
	Inventory = 7
};

// Forward declaration: BaseState существует, но полного описания не нужно
class BaseState;

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager {
public:
	explicit StateManager(SharedContext& context);
	~StateManager();

	void Update(const sf::Time& dT);
	void Draw();
	void ProcessRequests();

	SharedContext& GetContext();
	bool HasState(const StateType& type) const;

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type);

	sf::Cursor& GetCursor() { return m_cursor; }

private:
	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	// Шаблон оставляем для совместимости сигнатур, но тело пустое
	template<class T>
	void RegisterState(const StateType& /*type*/) {
		// В заглушке не регистрируем реальные состояния, 
		// чтобы не требовать наличия файлов State_*.h
	}

	SharedContext& m_context;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;

	sf::Image m_cursorImage;
	sf::Cursor m_cursor;
	bool m_cursorLoaded = false;
};

