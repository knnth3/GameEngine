#pragma once
#include <Graphics\Shape_Square.h>
#include "PathFinder.h"

//Everything that wants to be represented graphicsally
// will inherit as an actor

#define BLOCK_WIDTH 64.0f
#define BLOCK_HEIGHT 64.0f

enum ACTOR_LOOK_DIRECTION
{
	ACTOR_LOOK_DOWN,
	ACTOR_LOOK_LEFT,
	ACTOR_LOOK_RIGHT,
	ACTOR_LOOK_UP
};


class Actor
{
public:
	Actor();
	Actor(std::string name);

	void Update();

	void SetName(std::string name);
	void SetDestination(glm::vec2 pos);
	void SetPosition(glm::vec2 pos);
	void SetPosition(float width, float height);
	void SetBrush(int ID, Graphics::BRUSH_TEXTURE_TYPE type);
	void SetImageAtlasDivisons(int x, int y);
	void SetImageAtlasPosition(int x, int y);
	void SetDimensions(float x, float y);
	void SetMoveInstructions(NodePositions positions);

	std::string GetName()const;
	glm::vec2 GetPosition()const;
	glm::vec2 GetModelPosition()const;
	bool IsMoving()const;
	const Graphics::Square& Get()const;
	void GetDirection();



private:
	
	void SetMoveAnimation(int index);
	void SetRestAnimation(int index);
	glm::vec2 GridToWorld(Vector2<float> pos);
	glm::vec2 GridToWorld(glm::vec2 pos);
	void Animate();

	ACTOR_LOOK_DIRECTION m_lookDir;
	bool m_bNewCommands;
	bool m_bisMoving;
	std::vector<Vector2<int>> m_animList;
	NodePositions m_moves;
	std::string m_name;
	glm::vec2 m_destination;
	glm::vec2 m_gridPos;
	glm::vec2 m_animPos;
	Graphics::Square m_model;
};

