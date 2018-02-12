#include "pch.h"
#include "PathFinder.h"
#include <Lime_Engine\EngineResources.h>


template<typename T>
Vector2<T>::Vector2()
{
	x = T();
	y = T();
}

template<typename T>
Vector2<T>::Vector2(T xPos, T yPos)
{
	x = xPos;
	y = yPos;
}


PathFinder::PathFinder()
{
	m_start = nullptr;
	m_end = nullptr;
	m_gridWidth = 10;
	m_gridHeight = 10;
	m_nodeWidth = 32.0f;
	m_nodeHeight = 32.0f;
	m_gridWidth = 10;
	m_gridHeight = 10;
	m_startPos = Vector2<int>(0, 0);
	m_endPos = Vector2<int>(9, 9);
}

///////////Path Finder//////////////////////////////////////////////////////////////////////////////////////

bool PathFinder::FindPath(NodePositions& path)
{
	Initialize();
	m_openSet.Push_back(m_start);
	while (m_openSet.Size() > 0)
	{
		Node* currentNode = m_openSet.First();
		m_openSet.Pop_Front();
		m_closedSet.Push_back(currentNode);
		if (currentNode == m_end)
		{
			//Finished

			Retrace(currentNode, path);
			Close();
			return true;
		}

		for (auto& neighbor : currentNode->m_neighbors)
		{
			if (m_closedSet.Contains(neighbor) || !neighbor->bWalkable)
				continue;

			int newCost= currentNode->G_Cost + GetDistance(currentNode, neighbor);
			if (newCost < neighbor->G_Cost ||
				!m_openSet.Contains(neighbor))
			{
				neighbor->G_Cost = newCost;
				neighbor->H_Cost = GetDistance(neighbor, m_end);
				neighbor->ParentNode = currentNode;
				if (!m_openSet.Contains(neighbor))
				{
					m_openSet.Push_back(neighbor);
				}
			}
		}
	}

	Close();
	return false;
}

void PathFinder::SetNodeDimensions(float width, float height)
{
	m_nodeWidth = width;
	m_nodeHeight = height;
}

void PathFinder::SetGridDimensions(int width, int height)
{
	m_gridWidth = width;
	m_gridHeight = height;
}

void PathFinder::SetStartPoint(int x, int y)
{
	m_startPos = Vector2<int>(x, y);
}

void PathFinder::SetEndPoint(int x, int y)
{
	m_endPos = Vector2<int>(x, y);
}


void PathFinder::Initialize()
{
	m_grid.resize(m_gridWidth + 1);
	for (int x = 0; x < m_gridWidth + 1; x++)
	{
		for (int y = 0; y < m_gridHeight + 1; y++)
		{
			m_grid[x].emplace_back(new Node(x, y));
		}
	}
	for (int x = 0; x < m_gridWidth + 1; x++)
	{
		for (int y = 0; y < m_gridHeight + 1; y++)
		{
			m_grid[x][y]->AddNeighbors(m_grid);
		}
	}

	m_start = m_grid[m_startPos.x][m_startPos.y];
	m_end = m_grid[m_endPos.x][m_endPos.y];
}

void PathFinder::Close()
{
	for (auto& x : m_grid)
	{
		for (auto& y : x)
		{
			if (y)
			{
				delete y;
			}
		}
	}

	m_grid.clear();
	m_openSet.Clear();
	m_closedSet.Clear();
	m_start = nullptr;
	m_end = nullptr;

}

void PathFinder::Retrace(Node * end, NodePositions& path)
{
	NodePositions pathInv;
	Node* currentNode = end;

	while (currentNode != nullptr)
	{
		pathInv.emplace_back((float)currentNode->X_Position, (float)currentNode->Y_Position);
		currentNode = currentNode->ParentNode;
	}
	for (int i = (int)pathInv.size() - 1; i >= 0; i--)
	{
		path.push_back(pathInv[i]);
	}
}

int PathFinder::GetDistance(Node * a, Node* b)
{
	// 14 = cost of diag move
	// 10 = cost of everything else
	int distX = abs(a->X_Position - b->X_Position);
	int distY = abs(a->Y_Position - b->Y_Position);

	if (distX > distY)
		return 14 * distY + 10 * (distX - distY);

	return 14 * distX + 10 * (distY - distX);
}


///////////Path Finder//////////////////////////////////////////////////////////////////////////////////////


Node::Node(int x, int y, bool walkable)
{
	X_Position = x;
	Y_Position = y;
	G_Cost = 0;
	H_Cost = 0;
	bWalkable = walkable;
	ParentNode = nullptr;
}

int Node::F_Cost()const
{
	return G_Cost + H_Cost;
}

void Node::AddNeighbors(std::vector<std::vector<Node*>> arr)
{
	if(X_Position < arr.size() - 1)
		m_neighbors.push_back(arr[X_Position + 1][Y_Position]);

	if(X_Position > 0)
		m_neighbors.push_back(arr[X_Position - 1][Y_Position]);

	if(Y_Position < arr[0].size() - 1)
		m_neighbors.push_back(arr[X_Position][Y_Position + 1]);

	if(Y_Position > 0)
		m_neighbors.push_back(arr[X_Position][Y_Position - 1]);
}

NodeArray::NodeArray()
{
	std::make_heap(m_data.begin(), m_data.end(), greater1());
}

Node * NodeArray::First()
{
	return m_data.front();
}

void NodeArray::Push_back(Node * n)
{
	m_data.push_back(n);
	std::push_heap(m_data.begin(), m_data.end(), greater1());
}

void NodeArray::Pop_Front()
{
	std::pop_heap(m_data.begin(), m_data.end(), greater1());
	m_data.pop_back();
}

void NodeArray::Clear()
{
	m_data.clear();
}

size_t NodeArray::Size()
{
	return m_data.size();
}

bool NodeArray::Contains(Node * n)
{
	std::make_heap(m_data.begin(), m_data.end(), greater1());
	for (auto& item : m_data)
	{
		if (item == n)
		{
			return true;
		}
	}
	return false;
}

bool greater1::operator()(const Node * a, const Node * b) const
{
	if (a->F_Cost() == b->F_Cost())
	{
		if (a->H_Cost > b->H_Cost)
		{
			return false;
		}
		return a->H_Cost > b->H_Cost;
	}
	return a->F_Cost() > b->F_Cost();
}
