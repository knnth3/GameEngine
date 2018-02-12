#pragma once
#include <Graphics\Shape_Square.h>
#include "PathFinder.h"

//Everything that wants to be represented graphicsally
// will inherit as an actor

//Common Game Defines
#define BLOCK_WIDTH 64.0f
#define BLOCK_HEIGHT 64.0f
#define ANIM_DURATION 0.15


class Actor
{
public:
	Actor();
	Actor(std::string name);

	virtual void Update()final;
	virtual void SetName(std::string name)final;
	virtual void SetDestination(glm::vec2 pos)final;
	virtual void SetPosition(glm::vec2 pos)final;
	virtual void SetPosition(float width, float height)final;
	virtual void SetBrush(int ID, Graphics::BRUSH_TEXTURE_TYPE type)final;
	virtual void SetImageAtlasDivisons(int x, int y)final;
	virtual void SetImageAtlasPosition(int x, int y)final;
	virtual void SetDimensions(float x, float y)final;
	virtual void SetMoveInstructions(NodePositions positions)final;
	virtual void SetPassableState(bool val);

	virtual std::string GetName()const final;
	virtual glm::vec2 GetPosition()const final;
	virtual glm::vec2 GetDestination()const final;
	virtual glm::vec2 GetModelPosition()const final;
	virtual const Graphics::Square& Get()const final;
	virtual bool IsPassable()const final;

protected:
	virtual void Animate() = 0;
	virtual bool IsMoving()const final;
	virtual glm::vec2 GridToWorld(Vector2<float> pos)final;
	virtual glm::vec2 GridToWorld(glm::vec2 pos)final;
	virtual Vector2<int> GetImageAtlasDivisons()const final;
	virtual Vector2<int> GetImageAtlasPosition()const final;
	virtual float GetHeightOffset()const final;
	virtual void SetHeightOffset(float offset);

private:
	void Move();

	bool m_bNewCommands;
	bool m_bisMoving;
	bool m_bPassable;
	float m_heightOffset;
	std::vector<Vector2<int>> m_animList;
	NodePositions m_moves;
	std::string m_name;
	glm::vec2 m_destination;
	glm::vec2 m_gridPos;
	glm::vec2 m_animPos;
	Graphics::Square m_model;
};

