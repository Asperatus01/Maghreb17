#include "ai/path_finder.hpp"
#include <iostream>

PathFinder::PathFinder(SharedContext& context)
	: m_context(context)
	, m_startNode(nullptr)
	, m_goalNode(nullptr)
	, m_currentNode(nullptr)
{
	std::cerr << "[Stub] PathFinder created\n";
	// Не вызываем instantiateNodes() в заглушке,
	// чтобы не выделять 2500 узлов без нужды
	for (int r = 0; r < kMaxWorldRow; ++r)
		for (int c = 0; c < kMaxWorldCol; ++c)
			m_node[r][c] = nullptr;
}

PathFinder::~PathFinder() {
	std::cerr << "[Stub] PathFinder destroyed\n";
	// Аккуратно удаляем все узлы, если они были созданы
	for (int r = 0; r < kMaxWorldRow; ++r) {
		for (int c = 0; c < kMaxWorldCol; ++c) {
			delete m_node[r][c];
			m_node[r][c] = nullptr;
		}
	}
}

std::list<PathFinder::Node*> PathFinder::getPathList() {
	return m_pathList;
}

void PathFinder::setNodes(int /*startRow*/, int /*startCol*/,
	int /*goalRow*/, int /*goalCol*/) {
	// Заглушка: реальная логика установки узлов вернётся,
	// когда TileManager будет реализован
	std::cerr << "[Stub] PathFinder::setNodes called (no-op)\n";
}

bool PathFinder::search() {
	// Заглушка: путь не найден
	return false;
}

void PathFinder::instantiateNodes() {
	// Пустая заглушка
}

void PathFinder::resetNodes() {
	m_openList.clear();
	m_pathList.clear();
	m_goalReached = false;
	m_step = 0;
}

void PathFinder::getCost(Node* /*node*/) {
	// Пустая заглушка
}

void PathFinder::openNode(Node* /*node*/) {
	// Пустая заглушка
}

void PathFinder::trackThePath() {
	// Пустая заглушка
}
