// shared_context.hpp
#pragma once

#include <SFML/Graphics.hpp>

// Forward declarations Ч разрываем циклы включений
class Window;
class TileManager;
class EventManager;
//class StateManager;
class CollisionChecker;
class PathFinder;
class Player;
class AssetSetter;
class ControlPanel;
class MouseHandler;
class MusicPlayer;
class SoundPlayer;
//class AnimationHolder;
class Inventory;
class InventoryPanel;

class SharedContext {
public:
	SharedContext()
		: m_windPtr(nullptr)
		, m_tileManagerPtr(nullptr)
		, m_eventManagerPtr(nullptr)
		//, m_stateManagerPtr(nullptr)
		, m_checkerPtr(nullptr)
		, m_pathFinderPtr(nullptr)
		, m_playerPtr(nullptr)
		, m_assetSetterPtr(nullptr)
		, m_controlPanelPtr(nullptr)
		, m_inventoryPtr(nullptr)
		, m_iPanelPtr(nullptr)
		, m_mouseHandlerPtr(nullptr)
		, m_musicPlayerPtr(nullptr)
		, m_soundPlayerPtr(nullptr)
		//, m_animHolderPtr(nullptr)
		, m_renderWindowPtr(nullptr)
		, m_debug(false)
		, m_currentMap(0)
		, m_maxMap(10) {}

	// Ќ≈владеющие указатели (сырые): SharedContext не владеет объектами,
	// он только хранит ссылки на них. ”далением занимаетс€ Game.
	Window*                  m_windPtr;
	sf::RenderWindow*        m_renderWindowPtr;      // Ќ≈владеющий указатель на RenderWindow внутри Window

	TileManager*             m_tileManagerPtr;
	EventManager*            m_eventManagerPtr;
	//StateManager*           m_stateManagerPtr;
	CollisionChecker*        m_checkerPtr;
	PathFinder*              m_pathFinderPtr;
	Player*                  m_playerPtr;
	AssetSetter*             m_assetSetterPtr;
	ControlPanel*            m_controlPanelPtr;
	Inventory*               m_inventoryPtr;
	InventoryPanel*          m_iPanelPtr;
	MouseHandler*            m_mouseHandlerPtr;
	MusicPlayer*             m_musicPlayerPtr;
	SoundPlayer*             m_soundPlayerPtr;
	//AnimationHolder*         m_animHolderPtr;

	// ѕубличные флаги и счЄтчики (дл€ удобства доступа без геттеров в прототипе)
	bool m_debug;
	int  m_currentMap;
	int  m_maxMap;
};
