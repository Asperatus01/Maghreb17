#pragma once

#include "main/shared_context.hpp"
#include <vector>
#include <list>

class PathFinder {
public:
	struct Node {
		Node*  m_parent;
		int    m_row;
		int    m_col;
		double m_gCost;
		double m_hCost;
		double m_fCost;
		bool   m_open;
		bool   m_solid;
		bool   m_checked;

		Node(Node* parent = nullptr, int row = 0, int col = 0,
			double gCost = 0.0, double hCost = 0.0, double fCost = 0.0,
			bool open = false, bool solid = false, bool checked = false)
			: m_parent(parent)
			, m_row(row)
			, m_col(col)
			, m_gCost(gCost)
			, m_hCost(hCost)
			, m_fCost(fCost)
			, m_open(open)
			, m_solid(solid)
			, m_checked(checked) {}
	};

	explicit PathFinder(SharedContext& context);
	~PathFinder();

	std::list<Node*> getPathList();

	void setNodes(int startRow, int startCol, int goalRow, int goalCol);
	bool search();

private:
	void instantiateNodes();
	void resetNodes();
	void getCost(Node* node);
	void openNode(Node* node);
	void trackThePath();

	SharedContext& m_context;

	// Ћокальные константы дл€ заглушки Ч потом заменим на общие
	static constexpr int kMaxWorldRow = 50;
	static constexpr int kMaxWorldCol = 50;

	Node* m_node[kMaxWorldRow][kMaxWorldCol];
	Node* m_startNode;
	Node* m_goalNode;
	Node* m_currentNode;

	std::vector<Node*> m_openList;
	std::list<Node*>   m_pathList;

	bool m_goalReached = false;
	int  m_step = 0;
};

