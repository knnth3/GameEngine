#pragma once
#include "Entity.h"
#include <Lime\RenderBatch.h>


struct Character_Info
{
	Character_Info();
	bool isMoving;
	int current_health;
	int max_health;
	float move_speed;
	string character_name;
	string player_name;
	glm::vec3 position;
	glm::vec3 destination;
};

class Character:
	public Entity
{
public:
	Character(Character_Info info);
	void SetModel(MeshID id, shared_ptr<Lime::RenderBatch>& rend, shared_ptr<Lime::Camera>& camera);

	void Update(float time);
	void Draw();
	void SetLocation(glm::vec3 position);
	void SetDestination(glm::vec3 position);

private:
	void MovePlayer(float time);


	Character_Info m_info;

};

