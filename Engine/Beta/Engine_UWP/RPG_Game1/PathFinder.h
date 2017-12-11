#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <deque>

template <typename T>
struct Vector2
{
	Vector2();
	Vector2(T xPos, T yPos);
	T x;
	T y;
};

struct Node
{
	int X_Position;
	int Y_Position;
	int G_Cost;
	int H_Cost;
	bool bWalkable;
	std::vector<Node*> m_neighbors;
	Node* ParentNode;

	Node(int x, int y, bool walkable = true);
	int F_Cost()const;
	void AddNeighbors(std::vector<std::vector<Node*>> arr);

};

struct greater1 {
	bool operator()(const Node* a, const Node* b) const;
};

class NodeArray
{
public:
	NodeArray();
	Node* First();
	void Push_back(Node* n);
	void Pop_Front();
	void Clear();
	size_t Size();
	bool Contains(Node* n);

	//operator
	//Node* operator[](size_t index);
	//Node* operator[](int index);

private:

	std::vector<Node*> m_data;
};


typedef std::deque<Vector2<float>> NodePositions;

class PathFinder
{
public:
	PathFinder();
	bool FindPath(NodePositions& path);
	void SetNodeDimensions(float width, float height);
	void SetGridDimensions(int width, int height);
	void SetStartPoint(int x, int y);
	void SetEndPoint(int x, int y);

private:
	void Initialize();
	void Close();
	void Retrace(Node* end, NodePositions& path);
	int GetDistance(Node* a, Node* b);



	Vector2<int> m_startPos;
	Vector2<int> m_endPos;
	Node* m_start;
	Node* m_end;
	float m_nodeWidth, m_nodeHeight;
	int m_gridWidth, m_gridHeight;
	NodeArray m_openSet;
	NodeArray m_closedSet;
	std::vector<std::vector<Node*>> m_grid;
};